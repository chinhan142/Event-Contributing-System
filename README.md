# F-Code Event Staff Management System

The **F-Code Event Staff Management System** is a terminal-based application designed to streamline the management of club events and staff assignments. Developed for the **F-Code Academic Club** at **FPT University HCMC**, this system replaces manual tracking with a robust, file-based digital solution.

---

## 👥 Project Team: Glory Man United
* **Mentor:** Vo Dang Khoa
* **Team Members:**
    * **Bui Pham Chi Nhan** (SE203237) - Team Leader
    * **Tran Le Anh Quan** (SE200441)
    * **Vo Le Kien Huy** (SE210556)
    * **Tran Cao Thanh** (SE210041)
    * **Nguyen Tan Loi** (SE211059)

---

## 🛠 Technical Specifications
* **Programming Language:** C.
* **Data Storage:** Persistent storage using binary files (`.dat`) located in the `/data` folder.
* **Core Logic:** Implementation of nested structures, file I/O operations, and custom sorting algorithms.
* **Constraints:**
    * Support for up to 1,000,000 events.
    * Maximum of 30 staff members per event.
    * Role-based access control (Admin/BCN vs. Member/Staff).

---

## 📂 Project Structure
Based on the current project directory:
* `main.c`: Entry point and main menu navigation logic.
* `auth.h/.c`: Handles authentication, login sessions, and password security.
* `event.h/.c`: Core logic for creating and managing event data.
* `staff.h/.c`: Management of individual staff records and assignments within events.
* `menuBCN.h/.c`: Interface and menu options specifically for the Board of Management.
* `menuStaff.h/.c`: Interface and menu options for regular club members.
* `fileio.h/.c`: Handlers for reading and writing data to external files.
* `user.h/.c`: Logic related to user account profiles and details.
* `report.h/.c`: Statistics and report generation features (exporting to .txt).
* `init.h/.c`: System initialization and data loading protocols.
* `utils.h/.c`: Utility functions for input validation and date formatting.
* `colors.h`: ANSI escape codes for the console UI color scheme.
* `data/`: Directory containing system data files like `events.dat` and `accounts.dat`.

---

## 🚀 Installation & Execution
This project includes a dedicated build script to automate compilation using GCC.

### Prerequisites
* GCC Compiler.
* Bash environment (Linux, macOS, or Git Bash for Windows).

### How to Run
Open your terminal in the project root directory and execute the following command:
```bash
bash build.sh
