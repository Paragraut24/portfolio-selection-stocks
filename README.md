# Portfolio Selection System (DSAA - 4th Semester)

Course project for Data Structures and Algorithms (Semester 4).  
This repository contains:

- A C++ portfolio optimization system (knapsack variants + MST diversification)
- A Node.js/Express web app that uses the C++ backend
- Supporting docs, sample data, scripts, and Power BI notes

## Repository Structure

```text
portfolio-selection/
├── src/                  # CLI implementation (C++)
├── backend/              # Modular C++ backend used by web app
├── web-app/              # Node.js + frontend web interface
├── data/                 # Sample CSV datasets
├── docs/                 # Reports, outlines, sample runs
├── scripts/              # Utility scripts
├── powerbi/              # Dashboard setup notes
├── Makefile              # Build/test for CLI version
└── LICENSE
```

## Quick Start

### 1. C++ CLI Project

From repository root:

```bash
make
make run
make test
```

Main documentation: `docs/README.md`

### 2. Web App

```bash
cd web-app
npm install
copy .env.example .env
npm start
```

Then open: `http://localhost:3000`

Web app documentation: `web-app/README.md`

## Requirements

- C++17 compiler (`g++`, `clang++`, or MSVC equivalent)
- `make` (recommended for CLI build flow)
- Node.js 18+ and npm
- MySQL (for web app auth/database features)

## Security and Git Notes

- Do not commit `.env` files (already ignored).
- Commit `package-lock.json` for reproducible installs.
- Build binaries and generated outputs are ignored through `.gitignore`.

## License

MIT License. See `LICENSE`.
