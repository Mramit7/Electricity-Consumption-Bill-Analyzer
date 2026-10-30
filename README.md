# Electricity-Consumption-Bill-Analyzer
A high-performance Electricity Consumption &amp; Bill Analyzer featuring a pure C HTTP API backend (using native Windows Winsock with zero external dependencies) and a modern SaaS-inspired HTML5/CSS3/JavaScript frontend with glassmorphism UI.
# ⚡ WattWise: Electricity Consumption & Bill Analyzer

A lightweight, full-stack electricity bill calculation and analytics web application built with a **native C HTTP server backend** and a **modern SaaS-styled frontend**. 

Designed to demonstrate systems programming combined with web technologies, this project handles raw HTTP sockets, implements tiered slab-based utility pricing algorithms, and serves a responsive glassmorphic user interface without relying on heavy external web frameworks.

---

## ✨ Features

- **Pure C HTTP API Backend:** Built from scratch using Windows Winsock (`ws2_32`) sockets with manual HTTP parsing and CORS preflight support.
- **Tiered Slab Tariff Logic:** Automatically computes progressive energy charges based on consumption thresholds for both Residential and Commercial categories.
- **Comprehensive Cost Breakdown:** Itemizes energy charges, fixed service fees, fuel surcharges, and regulatory taxes.
- **Smart Recommendations:** Generates automated energy efficiency tips based on usage volume.
- **Modern UI/UX:** Styled with a midnight navy and teal glassmorphism design system for a premium user experience.

---

## 🛠️ Tech Stack

- **Backend:** C (Winsock, Standard C Libraries)
- **Frontend:** HTML5, CSS3 (CSS Variables, Flexbox/Grid, Glassmorphism), Vanilla JavaScript (`fetch` API)
- **Tooling:** GCC Compiler, Visual Studio Code

---

## 📂 Project Structure

```text
bill-analyzer/
│
├── server.c         # C Backend HTTP API Server
├── index.html       # Frontend User Interface
├── style.css        # SaaS-inspired Glassmorphism Styles
└── app.js           # Frontend Logic & API Integration
