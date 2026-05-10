#include "common.h"

double calculate_expected_return(const Stock& stock) {
    return stock.cost * (stock.return_pct / 100.0);
}
