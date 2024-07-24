# News Feeder Application

This project consists of a backend server written in Rust and a frontend GUI application using GTK in C. The backend serves news content, and the frontend displays it.

## Project Structure

- **backend/**: Contains the Rust backend server.
- **frontend/**: Contains the GTK-based C frontend application.

## Prerequisites

Before running the application, ensure you have the following installed:

- **Rust**: For building the backend server.
- **GTK**: For the frontend GUI.
- **libcurl**: For making HTTP requests in the frontend.
- **libjson-c**: For JSON parsing in the frontend.
- **gcc**: For compiling the GTK frontend.

### Installing Dependencies

#### Rust

You can install Rust using [rustup](https://rustup.rs/):

```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
