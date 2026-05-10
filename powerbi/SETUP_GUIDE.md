# Power BI Setup Guide

## Overview

This guide walks you through setting up Power BI dashboards for the Portfolio Selection System.

## Prerequisites

1. Power BI Desktop (Free download: https://powerbi.microsoft.com/desktop/)
2. CSV files generated from the portfolio system
3. (Optional) Power BI Service account for publishing

## Step 1: Prepare Data Files

Run the portfolio optimization to generate CSV files:

```bash
cd portfolio-selection/backend
./portfolio_backend.exe stocks.csv 5000
```

This creates:
- `stocks.csv` - Input stock data
- `portfolio_results_2d.csv` - 2D DP results
- `portfolio_results_1d.csv` - 1D DP results
- `portfolio_results_greedy.csv` - Greedy algorithm results
- `portfolio_results_fractional.csv` - Fractional knapsack results
- `algorithm_comparison.csv` - Algorithm comparison metrics
- `diversification_output.csv` - MST diversification data

## Step 2: Import Data into Power BI

1. Open Power BI Desktop
2. Click "Get Data" → "Text/CSV"
3. Import each CSV file:
   - Navigate to `portfolio-selection/backend/`
   - Select CSV file
   - Click "Load"
4. Repeat for all CSV files

## Step 3: Create Relationships

In Power BI, create relationships between tables:

1. Go to "Model" view
2. Create relationships:
   - `stocks.id` → `portfolio_results_*.stock_id`
   - Link all result tables to stocks table

## Step 4: Create Visualizations

### Dashboard 1: Portfolio Overview

**Page Layout:**
- Title: "Portfolio Selection Dashboard"
- Filters: Algorithm selector, Budget range

**Visualizations:**

1. **Portfolio Distribution (Pie Chart)**
   - Values: cost
   - Legend: stock_name
   - Source: portfolio_results_2d.csv

2. **Expected Returns (Card)**
   - Value: SUM(expected_return_amount)
   - Source: portfolio_results_2d.csv

3. **Total Investment (Card)**
   - Value: SUM(cost)
   - Source: portfolio_results_2d.csv

4. **Number of Stocks (Card)**
   - Value: COUNT(stock_id)
   - Source: portfolio_results_2d.csv

### Dashboard 2: Algorithm Comparison

**Visualizations:**

1. **Profit Comparison (Clustered Bar Chart)**
   - Axis: algorithm
   - Values: total_expected_return, total_cost
   - Source: algorithm_comparison.csv

2. **Runtime Performance (Bar Chart)**
   - Axis: algorithm
   - Values: runtime_ms
   - Source: algorithm_comparison.csv

3. **Stocks Selected (Bar Chart)**
   - Axis: algorithm
   - Values: stocks_selected
   - Source: algorithm_comparison.csv

4. **Comparison Table**
   - Columns: algorithm, stocks_selected, total_cost, total_expected_return, runtime_ms
   - Source: algorithm_comparison.csv

### Dashboard 3: Risk Analysis

**Visualizations:**

1. **Risk vs Return (Scatter Plot)**
   - X-axis: risk_score
   - Y-axis: expected_return_percent
   - Details: name
   - Size: cost
   - Source: stocks.csv

2. **Risk Distribution (Histogram)**
   - Axis: risk_score (binned)
   - Values: COUNT(id)
   - Source: stocks.csv

3. **Return Distribution (Histogram)**
   - Axis: expected_return_percent (binned)
   - Values: COUNT(id)
   - Source: stocks.csv

4. **Stock Performance Table**
   - Columns: name, cost, expected_return_percent, risk_score
   - Source: stocks.csv

### Dashboard 4: Diversification Analysis

**Visualizations:**

1. **MST Network (Table)**
   - Columns: stock1_name, stock2_name, weight
   - Source: diversification_output.csv

2. **Diversification Metrics (Cards)**
   - Total Weight: SUM(weight)
   - Edge Count: COUNT(*)
   - Source: diversification_output.csv

## Step 5: Add Calculated Measures

Create DAX measures for advanced analytics:

```dax
// Total Return Percentage
Total Return % = 
DIVIDE(
    SUM(portfolio_results_2d[expected_return_amount]),
    SUM(portfolio_results_2d[cost])
) * 100

// Average Risk
Average Risk = 
AVERAGE(stocks[risk_score])

// Risk-Adjusted Return
Risk Adjusted Return = 
DIVIDE(
    SUM(portfolio_results_2d[expected_return_percent]),
    AVERAGE(stocks[risk_score])
)

// Efficiency Ratio
Efficiency Ratio = 
DIVIDE(
    SUM(algorithm_comparison[total_expected_return]),
    SUM(algorithm_comparison[runtime_ms])
)
```

## Step 6: Format Dashboard

1. **Theme:**
   - Use custom colors: #667eea (primary), #764ba2 (secondary)
   - Background: White or light gradient

2. **Fonts:**
   - Title: Segoe UI Bold, 24pt
   - Headers: Segoe UI Semibold, 16pt
   - Body: Segoe UI, 12pt

3. **Layout:**
   - Use consistent spacing
   - Align visualizations in grid
   - Add borders and shadows for depth

## Step 7: Publish to Power BI Service

1. Click "Publish" in Power BI Desktop
2. Sign in to Power BI Service
3. Select workspace
4. Click "Publish"

## Step 8: Embed in Web Application

1. In Power BI Service, open your report
2. Click "File" → "Embed report" → "Website or portal"
3. Copy the embed URL
4. Paste into web application's Power BI page
5. Click "Embed Dashboard"

## Data Refresh

### Manual Refresh
1. Re-run portfolio optimization
2. In Power BI Desktop, click "Refresh"
3. Re-publish to Power BI Service

### Automatic Refresh (Power BI Service)
1. Upload CSV files to OneDrive/SharePoint
2. Configure scheduled refresh in Power BI Service
3. Set refresh frequency (daily, hourly, etc.)

## Sample DAX Queries

### Top 5 Stocks by Return
```dax
Top 5 Stocks = 
TOPN(
    5,
    stocks,
    stocks[expected_return_percent],
    DESC
)
```

### Algorithm Efficiency Score
```dax
Efficiency Score = 
DIVIDE(
    [Total Return %],
    [Average Risk]
) * 100
```

### Portfolio Diversity Index
```dax
Diversity Index = 
DISTINCTCOUNT(portfolio_results_2d[stock_id]) / 
COUNTROWS(stocks)
```

## Troubleshooting

### Data Not Loading
- Check CSV file paths
- Verify CSV format (comma-separated, UTF-8)
- Ensure headers match expected column names

### Relationships Not Working
- Verify ID columns are same data type
- Check for null values
- Ensure cardinality is correct (many-to-one)

### Visualizations Not Showing
- Check data types (numeric vs text)
- Verify aggregation settings
- Clear filters and try again

## Best Practices

1. **Data Quality:**
   - Validate CSV files before import
   - Handle missing values
   - Use consistent naming conventions

2. **Performance:**
   - Limit data to necessary columns
   - Use aggregations where possible
   - Optimize DAX queries

3. **Design:**
   - Keep dashboards simple and focused
   - Use consistent color scheme
   - Add tooltips for context

4. **Security:**
   - Don't embed sensitive data
   - Use row-level security if needed
   - Regularly review access permissions

## Resources

- Power BI Documentation: https://docs.microsoft.com/power-bi/
- DAX Reference: https://dax.guide/
- Power BI Community: https://community.powerbi.com/
- Sample Reports: https://powerbi.microsoft.com/en-us/sample-reports/
