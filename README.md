# Frequent Flyer Program (FFP)
SEHH2242 Group Project


## Code Structure
The program uses a **clear, modular structure** designed for group collaboration.

## Group Work Logic
- Public area: shared and reusable
- Each R function: independent work
- No conflict, clean, easy to maintain
- 
### 1. Header Files
Include all required libraries for input, string, and data processing.
- `iostream` – Input/output
- `string` – String operations
- `vector` / `algorithm` – Data handling
- `cctype` – Character validation
- `cstdlib` & `ctime` – System functions

### 2. Public Common Area
Shared resources used by all functions:
- Constants (system limits)
- Two OOP classes: `Member` and `Flight`
- Global data (members, flights, system date)
- Public helper functions (validation, search, calculation, formatting)

### 3. Requirement Functions (R0 – R6)
Each member implements one module:
- R0: Show main menu
- R1: Load initial data
- R2: Display all members
- R3: Open / close account
- R4: Member operations (edit info, update points, add flight, redeem & transfer)
- R5: Generate daily statement
- R6: Exit program

### 4. Main Function
Controls program flow, menu selection, and data access control.
- Must run **R1 first** to load data before using R2–R5.



