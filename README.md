# F-Code Event Staff Management System

<div align="center">

**A Terminal-Based Event & Staff Management System for F-Code Club**

[![Language](https://img.shields.io/badge/Language-C-blue.svg?style=flat-square)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-brightgreen.svg?style=flat-square)](https://gcc.gnu.org/)

</div>

---

## 📋 Overview

The **F-Code Event Staff Management System** is a terminal-based application designed to streamline the management of club events and staff assignments. Developed for the **F-Code Academic Club** at **FPT University HCMC**, this system replaces manual tracking with a robust, file-based digital solution.

**Key Highlights:**
- 🔐 **Role-based Access Control** - Admin (BCN) and Member (Staff) accounts
- 📊 **Event Management** - Create, edit, and track club events with detailed staff assignments
- 👥 **Staff Management** - Manage staff records and assignments with up to 1M events support
- 💾 **Persistent Storage** - Binary file-based data persistence with automatic data loading
- 📈 **Reporting** - Generate and export event statistics to text files
- 🎨 **Colored UI** - Enhanced terminal interface with ANSI color codes

---

## 👥 Project Team: Glory Man United

**Mentor:** Vo Dang Khoa

**Team Members:**
| Name | Student ID | Role |
|------|-----------|------|
| Bui Pham Chi Nhan | SE203237 | Team Leader |
| Tran Le Anh Quan | SE200441 | Developer |
| Vo Le Kien Huy | SE210556 | Developer |
| Tran Cao Thanh | SE210041 | Developer |
| Nguyen Tan Loi | SE211059 | Developer |

---

## 🛠 Technical Specifications

| Aspect | Details |
|--------|---------|
| **Language** | C (ISO C standard) |
| **Build System** | GCC Compiler + Makefile |
| **Data Storage** | Binary files (`.dat`) in `/data` folder |
| **Architecture** | Modular design with header/source separation |
| **Max Events** | 1,000,000 events |
| **Max Staff/Event** | 30 staff members per event |
| **Access Control** | Role-based (Admin/BCN, Member/Staff) |

---

## 📂 Project Structure

```
EventSystem/
├── main.c                    # Application entry point
├── Makefile                  # Build configuration (Windows)
├── build.sh                  # Build script (Linux/macOS)
├── colors.h                  # ANSI color codes for UI
├── init.c/h                  # System initialization
├── paths.h                   # File path constants
│
├── include/                  # Header files
│   ├── app.h                # Main application interface
│   ├── auth.h               # Authentication & login
│   ├── event.h              # Event data structures & logic
│   ├── staff.h              # Staff management
│   ├── user.h               # User account profiles
│   ├── menuBCN.h            # Board management menus
│   ├── menuSearch.h         # Search functionality
│   ├── menuStaff.h          # Staff member menus
│   ├── fileio.h             # File I/O operations
│   ├── report.h             # Report generation
│   ├── utils.h              # Utility functions
│   └── colors.h             # UI colors
│
├── src/                      # Source files
│   ├── app.c                # Main application logic
│   ├── auth.c               # Authentication implementation
│   ├── event.c              # Event management
│   ├── staff.c              # Staff operations
│   ├── user.c               # User profiles
│   ├── menuBCN.c            # Admin menu interface
│   ├── menuSearch.c         # Search implementation
│   ├── menuStaff.c          # Member menu interface
│   ├── fileio.c             # File reading/writing
│   ├── report.c             # Report generation
│   ├── utils.c              # Utility implementations
│   └── init.c               # System initialization
│
├── data/                     # Data storage
│   ├── exports/             # Generated reports
│   ├── events.dat           # Event database
│   └── accounts.dat         # User accounts
│
└── tests/                    # Test files
    ├── 1m_event_tests/      # Large-scale performance tests
    └── staff/               # Staff module tests
```

### Module Descriptions

| Module | Purpose |
|--------|---------|
| **auth** | User login, session management, password handling |
| **event** | Event creation, editing, deletion, and core logic |
| **staff** | Staff records, role assignment, availability tracking |
| **user** | User account profiles and permissions |
| **menuBCN** | Administrative menu for board management |
| **menuStaff** | User menu for regular club members |
| **fileio** | Binary file read/write operations, data serialization |
| **report** | Event statistics, export to text format |
| **utils** | Input validation, string processing, date formatting |
| **init** | Application startup, data initialization |

---

## 🚀 Getting Started

### Prerequisites

Ensure you have the following installed:

- **GCC Compiler** (version 7.0 or higher)
  - [Windows (MinGW)](https://www.mingw-w64.org/)
  - Linux: `sudo apt-get install build-essential`
  - macOS: `xcode-select --install`

- **Bash** (or equivalent shell)
  - Windows: Git Bash, WSL, or use Makefile
  - Linux/macOS: Built-in

### Installation Steps

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd EventSystem
   ```

2. **Run the build script:**
   ```bash
   bash build.sh
   ```

The script will automatically compile and launch the application.

---

## 💻 Usage

The application starts automatically after successful compilation. Follow the on-screen menus to navigate.

### User Accounts

The system provides two access levels:

- **Admin (BCN)** - Board of Management
  - Full event creation and management
  - Staff assignment and approval
  - Report generation
  - User account management

- **Member (Staff)** - Regular Club Member
  - View assigned events
  - Update availability status
  - View event details
  - Limited report access

### Account Registration

**New club members can register their own account:**

When you start the application, you will see the login menu with three options:

1. Select **Option 2: Register**
2. Enter your **Student ID** in the format `SExxxxxx` (where x is a digit, e.g., `SE203237`)
3. Enter your **Full Name**
4. Enter your **Email Address**
5. Create a **password** for your account
6. Confirm your password
7. Your account and user profile will be automatically saved and ready to use

**Registration Requirements:**
- ✅ Student ID must follow the format: `SExxxxxx` (SE + 6 digits)
- ✅ Full Name required
- ✅ Email address required
- ✅ Password must be confirmed
- ✅ Each Student ID can only be registered once
- ✅ New accounts are created with **Member (Staff)** role by default
- ✅ User profile is automatically created for event assignment
- ✅ All registration data is automatically saved to `accounts.dat` and `users.dat`

### Default Login Credentials

When you run the application for the first time, the system initializes with pre-configured accounts. Use the following credentials to log in:

#### Admin Account (BCN)
| Credential | Value |
|------------|-------|
| **Username** | `admin` |
| **Password** | `admin` |
| **Role** | Board of Management |

#### Staff Accounts (Member)
| Username | Password | Role |
|----------|----------|------|
| `SE111111` | `SE111111` | Staff Member |
| `SE203237` | `SE203237` | Staff Member |

**How to Log In:**

1. Start the application: `bash build.sh`
2. At the login menu, select **Option 1: Login**
3. Enter your username (Student ID or `admin`)
4. Enter your password
5. The system will verify your credentials and grant access based on your role

### Default Data

- Initial user accounts and events are loaded from `/data/accounts.dat` and `/data/events.dat`
- If files don't exist, the system initializes default data
- All changes are automatically saved to binary files

### Generated Reports

- Event statistics and summaries are exported to `/data/exports/` directory
- Export filenames include timestamps: `events_all_YYYYMMDD_HHMMSS.txt`

---

## 🧪 Testing

### Unit Tests

Located in the `tests/` directory:

```bash
# Test staff module
cd tests/staff
bash build_staff.sh

# Test with large datasets (1M events)
cd tests/1m_event_tests
gcc generate_1mevents.c -Iinclude -o generate.exe
./generate.exe
```

### Performance Benchmarks

Testcase includes 1M event benchmark:

```bash
cd testcase/1m_event_tests
gcc benchmark_menu_funcs_1m.c -o benchmark.exe -Iinclude
./benchmark.exe
```

---

## 📋 System Features

### Event Management
- ✅ Create new events with details (name, date, location, capacity)
- ✅ Edit event information
- ✅ Delete events
- ✅ Search events by criteria
- ✅ Sort events by multiple fields

### Staff Management
- ✅ Add/remove staff members
- ✅ Assign staff to events
- ✅ Track attendance and availability
- ✅ Update staff roles

### User Management
- ✅ User authentication with password
- ✅ Role-based access control
- ✅ Account creation and deletion
- ✅ Session management

### Reporting
- ✅ Generate event statistics
- ✅ Export reports to text format
- ✅ Filter reports by date range
- ✅ Attendance tracking reports

### Data Management
- ✅ Persistent binary file storage
- ✅ Automatic data backup on modification
- ✅ Data integrity checks
- ✅ File corruption recovery

---

## 🔧 Troubleshooting

| Issue | Solution |
|-------|----------|
| **GCC not found** | Install MinGW (Windows) or build-essential (Linux) |
| **Permission denied** | Use `chmod +x build.sh` on Linux/macOS |
| **Build fails** | Ensure all include files are in the `include/` folder |
| **Data files missing** | Delete `data/` folder and restart to reinitialize |
| **Bash not found (Windows)** | Use Git Bash, WSL, or run Makefile instead |

---

## 📝 Coding Standards

- **Style Guide:** Follow Linux kernel coding style
- **Comments:** English comments for public interfaces
- **Functions:** Descriptive names with lowercase and underscores
- **Variables:** Clear, self-documenting names
- **Error Handling:** Return error codes; check before use

---

## 🤝 Contributing

When contributing to this project:

1. Follow the existing code style
2. Add unit tests for new features
3. Update documentation for changes
4. Use descriptive commit messages
5. Test on both Windows and Linux before submitting

---

## 📄 License

This project is developed for academic purposes at FPT University.

---

## 📧 Contact

For questions or support, contact the team leader:
- **Bui Pham Chi Nhan** (SE203237)

---

**Last Updated:** May 2026  
**Project Status:** Active Development
