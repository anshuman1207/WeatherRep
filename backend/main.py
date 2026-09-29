from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import psycopg2

app = FastAPI()

# Allow CORS for the React dashboard
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class SensorData(BaseModel):
    temperature: float
    humidity: float

def get_db_connection():
    # Peer authentication via Unix socket (no password required)
    return psycopg2.connect(dbname="weather_db")

@app.get("/")
def read_root():
    return {"status": "WeatherRep API is running"}

@app.post("/sensor-data")
def receive_data(data: SensorData):
    conn = get_db_connection()
    cur = conn.cursor()
    cur.execute(
        "INSERT INTO sensor_readings (temperature, humidity) VALUES (%s, %s)",
        (data.temperature, data.humidity)
    )
    conn.commit()
    cur.close()
    conn.close()
    return {"message": "Data saved successfully"}

@app.get("/sensor-data")
def get_data():
    conn = get_db_connection()
    cur = conn.cursor()
    cur.execute("SELECT * FROM sensor_readings ORDER BY id DESC LIMIT 10")
    rows = cur.fetchall()
    cur.close()
    conn.close()
    
    formatted_data = []
    for row in rows:
        formatted_data.append({
            "id": row[0],
            "temperature": row[1],
            "humidity": row[2],
            "timestamp": row[3] if len(row) > 3 else None
        })
    return formatted_data
