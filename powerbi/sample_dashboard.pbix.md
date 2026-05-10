# Power BI Sample Dashboard Template

## File Information

This document describes the structure of a sample Power BI dashboard for the Portfolio Selection System.

**Note:** Actual `.pbix` file should be created in Power BI Desktop following the SETUP_GUIDE.md instructions.

## Dashboard Structure

### Page 1: Executive Summary

**Layout:** 4x3 grid

**Components:**
1. **Header (Full Width)**
   - Title: "Portfolio Selection Dashboard"
   - Subtitle: "Optimization Results & Analytics"
   - Last Updated: Dynamic timestamp

2. **KPI Cards (Row 1)**
   - Total Investment (Rs.)
   - Expected Return (Rs.)
   - Return Percentage (%)
   - Number of Stocks

3. **Main Visualizations (Row 2-3)**
   - Portfolio Distribution (Pie Chart) - 2 columns
   - Top 5 Stocks by Return (Bar Chart) - 2 columns
   - Risk vs Return Scatter (Scatter Plot) - 4 columns

4. **Filters Panel (Right Side)**
   - Algorithm Selector
   - Budget Range Slider
   - Risk Level Filter

### Page 2: Algorithm Comparison

**Layout:** 3x3 grid

**Components:**
1. **Comparison Table (Top)**
   - All algorithms with metrics
   - Sortable columns
   - Conditional formatting

2. **Performance Charts (Middle)**
   - Return Comparison (Clustered Bar)
   - Runtime Comparison (Bar)
   - Efficiency Ratio (Line)

3. **Detailed Metrics (Bottom)**
   - Time Complexity Cards
   - Space Complexity Cards
   - Optimality Indicators

### Page 3: Risk Analysis

**Layout:** 2x2 grid

**Components:**
1. **Risk Distribution (Histogram)**
2. **Return Distribution (Histogram)**
3. **Risk-Return Matrix (Scatter with Quadrants)**
4. **Stock Performance Table**

### Page 4: Diversification

**Layout:** Custom

**Components:**
1. **MST Network Visualization**
   - Edge table with weights
   - Connected stocks
2. **Diversification Metrics**
   - Total MST weight
   - Number of edges
   - Average correlation

## Data Model

### Tables

1. **stocks**
   - id (Key)
   - name
   - cost
   - expected_return_percent
   - risk_score

2. **portfolio_results_2d**
   - stock_id (Foreign Key)
   - stock_name
   - cost
   - expected_return_percent
   - expected_return_amount
   - risk_score

3. **portfolio_results_1d** (Same structure as 2d)
4. **portfolio_results_greedy** (Same structure as 2d)
5. **portfolio_results_fractional** (Same structure as 2d)

6. **algorithm_comparison**
   - algorithm (Key)
   - stocks_selected
   - total_cost
   - total_expected_return
   - avg_return_percent
   - runtime_ms

7. **diversification_output**
   - stock1_id
   - stock1_name
   - stock2_id
   - stock2_name
   - weight

### Relationships

```
stocks (1) → (*) portfolio_results_2d [id → stock_id]
stocks (1) → (*) portfolio_results_1d [id → stock_id]
stocks (1) → (*) portfolio_results_greedy [id → stock_id]
stocks (1) → (*) portfolio_results_fractional [id → stock_id]
```

## Measures (DAX)

```dax
Total Investment = SUM(portfolio_results_2d[cost])

Expected Return = SUM(portfolio_results_2d[expected_return_amount])

Return Percentage = DIVIDE([Expected Return], [Total Investment]) * 100

Average Risk = AVERAGE(stocks[risk_score])

Risk Adjusted Return = DIVIDE([Return Percentage], [Average Risk])

Stock Count = DISTINCTCOUNT(portfolio_results_2d[stock_id])

Best Algorithm = 
CALCULATE(
    FIRSTNONBLANK(algorithm_comparison[algorithm], 1),
    TOPN(1, algorithm_comparison, algorithm_comparison[total_expected_return], DESC)
)

Efficiency Score = 
DIVIDE(
    algorithm_comparison[total_expected_return],
    algorithm_comparison[runtime_ms]
)
```

## Color Scheme

- Primary: #667eea (Purple-Blue)
- Secondary: #764ba2 (Purple)
- Success: #28a745 (Green)
- Warning: #ffc107 (Yellow)
- Danger: #dc3545 (Red)
- Background: #ffffff (White)
- Text: #333333 (Dark Gray)

## Formatting Guidelines

1. **Numbers:**
   - Currency: Rs. #,##0.00
   - Percentage: 0.00%
   - Whole Numbers: #,##0

2. **Fonts:**
   - Title: Segoe UI Bold, 24pt
   - Subtitle: Segoe UI Semibold, 18pt
   - Body: Segoe UI, 12pt
   - Small: Segoe UI, 10pt

3. **Spacing:**
   - Padding: 10px
   - Margin: 15px
   - Border Radius: 8px

## Interactive Features

1. **Cross-Filtering:**
   - Click on pie chart slice → filters all visuals
   - Select algorithm → shows specific results

2. **Drill-Through:**
   - Right-click stock → view detailed analysis
   - Drill from summary to detailed view

3. **Tooltips:**
   - Hover over data points → show details
   - Custom tooltip pages for complex data

4. **Bookmarks:**
   - Save different views
   - Quick navigation between scenarios

## Export Options

1. **PDF:** Full dashboard export
2. **PowerPoint:** Slide-by-slide export
3. **Excel:** Data export with formatting
4. **Image:** PNG export for reports

## Refresh Schedule

- **Development:** Manual refresh
- **Production:** Daily at 6:00 AM
- **On-Demand:** Via web application trigger

## Access Control

- **Viewers:** Read-only access to dashboards
- **Contributors:** Can edit and publish
- **Admins:** Full control including data sources

## Mobile Layout

Optimized mobile view with:
- Stacked visualizations
- Simplified KPIs
- Touch-friendly filters
- Responsive design

## Notes

- Create this dashboard in Power BI Desktop
- Save as `portfolio_dashboard.pbix`
- Place in `portfolio-selection/powerbi/` directory
- Follow SETUP_GUIDE.md for detailed instructions
