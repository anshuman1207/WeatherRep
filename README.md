# 🌦️ WeatherRep: Full-Stack IoT Environmental Monitor

![React](https://img.shields.io/badge/react-%2320232a.svg?style=for-the-badge&logo=react&logoColor=%2361DAFB)
![FastAPI](https://img.shields.io/badge/FastAPI-005571?style=for-the-badge&logo=fastapi)
![PostgreSQL](https://img.shields.io/badge/postgresql-4169e1?style=for-the-badge&logo=postgresql&logoColor=white)
![Docker](https://img.shields.io/badge/docker-%230db7ed.svg?style=for-the-badge&logo=docker&logoColor=white)
![AWS](https://img.shields.io/badge/AWS-%23FF9900.svg?style=for-the-badge&logo=amazon-aws&logoColor=white)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

WeatherRep is a full-stack Internet of Things (IoT) application. It collects real-time environmental telemetry from a physical microchip, transmits it securely over the internet, stores it in a relational database, and visualizes it on a live web dashboard.

## 🏗️ System Architecture

1. **The Edge (Hardware):** An ESP32 microcontroller paired with a DS18B20 digital temperature probe (using 1-Wire protocol). The ESP32 connects to Wi-Fi and sends HTTP POST requests containing JSON data.
2. **The Brains (Backend):** A Python FastAPI REST API running inside a Docker container on an AWS EC2 Ubuntu instance.
3. **The Memory (Database):** A PostgreSQL database running on the EC2 host, connected to the Docker container via Unix socket mapping and peer authentication.
4. **The Face (Frontend):** A React single-page application (scaffolded with Vite), polling the API asynchronously. Deployed statically using an Nginx web server on AWS.

## 📂 Project Structure

```text
WeatherRep/
├── backend/                  # Python FastAPI API & Docker configuration
│   ├── main.py               # REST API endpoints & DB connection
│   ├── requirements.txt      # Python dependencies
│   └── Dockerfile            # Container build instructions
├── frontend/                 # (iot-dashboard) React + Vite web app
│   ├── src/                  # React components and styling
│   └── package.json          # Node dependencies
└── hardware/                 # C++ ESP32 Code
    └── WeatherRep_Node/
        └── WeatherRep_Node.ino # 1-Wire sensor reading & HTTP POST logic
```

## 🚀 How It Was Built (The 7-Day Journey)

* **Day 1 (Cloud Networking):** Provisioned an AWS EC2 instance (`t3.micro`), configured SSH keys, and set up Security Groups to allow Port 22 (SSH) and Port 80 (HTTP).
* **Day 2 (REST API):** Developed a Python FastAPI backend to receive sensor data and serve it via GET/POST endpoints. Enabled `CORSMiddleware`.
* **Day 3 (Database):** Installed PostgreSQL on Linux. Created the `weather_db` and `sensor_readings` table. Linked the API to the DB using `psycopg2`.
* **Day 4 (IoT Hardware):** Wired an ESP32 and wrote C++ code to connect to local Wi-Fi and push JSON payloads to the AWS cloud. Debugged hardware pull-up resistor requirements for the DS18B20.
* **Day 5 (Frontend):** Built a React dashboard with Vite to asynchronously fetch and render the live cloud data using `useEffect` and `useState`.
* **Day 6 (DevOps & Deployment):** Packaged the FastAPI backend into an auto-restarting Docker container (mapping `/var/run/postgresql` and `/etc/passwd` to bypass socket/permission isolation). Deployed the React dashboard to the public internet using Nginx.

## ⚙️ Hardware Requirements
* ESP32 Development Board
* DS18B20 Waterproof Temperature Sensor
* 4.7kΩ Pull-up Resistor (Required between 3.3V and Data pins for 1-Wire communication)
