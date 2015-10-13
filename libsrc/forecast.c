#include "kary.h"
#include "forecast.h"

void
MA(uint64_t *sketch_table, double *forecast_table, int interval){
	int hash_counter, key_counter, interval_counter;

	uint64_t KEY;
	KEY = (1L << KEY_bit);

	for( hash_counter = 0; hash_counter < HASH; hash_counter++ ){
		for( key_counter = 0; key_counter < KEY; key_counter++ ){
			*( forecast_table + hash_counter*KEY + key_counter ) = 0;
		}
	}
	
	for( hash_counter = 0; hash_counter < HASH; hash_counter++ ){
		for( key_counter = 0; key_counter < KEY; key_counter++ ){
			for( interval_counter = 0; interval_counter < interval; interval_counter++ ){
				*( forecast_table + hash_counter*KEY + key_counter ) += \
				((double)*( sketch_table + interval_counter*HASH*KEY + hash_counter*KEY + key_counter ));
			}
			for( interval_counter = (interval+1); interval_counter < (WINDOWS+1); interval_counter++ ){
				*( forecast_table + hash_counter*KEY + key_counter ) += \
				((double)*( sketch_table + interval_counter*HASH*KEY + hash_counter*KEY + key_counter ));
			}
			*( forecast_table + hash_counter*KEY + key_counter ) = ( *( forecast_table + hash_counter*KEY + key_counter ) / (double)(WINDOWS));
		}
	}


}

void
MA2(uint64_t *sketch_table, double *forecast_table, int interval){
	int hash_counter, key_counter, interval_counter;

	uint64_t KEY;
	KEY = (1L << KEY_bit);

	for( hash_counter = 0; hash_counter < HASH; hash_counter++ ){
		for( key_counter = 0; key_counter < KEY; key_counter++ ){
			*( forecast_table + hash_counter*KEY + key_counter ) = 0;
		}
	}
	
	for( hash_counter = 0; hash_counter < HASH; hash_counter++ ){
		for( key_counter = 0; key_counter < KEY; key_counter++ ){
			if( interval == WINDOWS ){
				for( interval_counter = 1; interval_counter < interval; interval_counter++ ){
					*( forecast_table + hash_counter*KEY + key_counter ) += \
					((double)*( sketch_table + interval_counter*HASH*KEY + hash_counter*KEY + key_counter ));
				}
			}
			else {
				for( interval_counter = 0; interval_counter < interval; interval_counter++ ){
					*( forecast_table + hash_counter*KEY + key_counter ) += \
					((double)*( sketch_table + interval_counter*HASH*KEY + hash_counter*KEY + key_counter ));
				}
			}
			for( interval_counter = (interval+2); interval_counter < (WINDOWS+1); interval_counter++ ){
				*( forecast_table + hash_counter*KEY + key_counter ) += \
				((double)*( sketch_table + interval_counter*HASH*KEY + hash_counter*KEY + key_counter ));
			}
			*( forecast_table + hash_counter*KEY + key_counter ) = ( *( forecast_table + hash_counter*KEY + key_counter ) / (double)(WINDOWS_2));
		}
	}


}

void MA1(uint64_t *sketch_table, double *forecast_table){
	int interval=1;
	int hash_counter, key_counter;
	uint64_t KEY;
	KEY = (1L << KEY_bit);

	for( hash_counter = 0; hash_counter < HASH; hash_counter++ ){
		for( key_counter = 0; key_counter < KEY; key_counter++ ){
			*( forecast_table + hash_counter*KEY + key_counter ) = (double) *( sketch_table + hash_counter*KEY + key_counter );
		}
	}


}
