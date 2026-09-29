import { useState, useEffect } from 'react'
import './App.css'

function App() {
  // 1. Set up React's Memory (State)
  const [sensorData, setSensorData] = useState([]);
  const [loading, setLoading] = useState(true);

  // 2. Function to fetch data from AWS API
  const fetchData = async () => {
    try {
      // We are acting like the Swagger UI "Try it out" button!
      const response = await fetch("http://13.204.77.140:8000/sensor-data");
      const data = await response.json();
      
      // Save the history array into React's memory
      setSensorData(data.history); 
      setLoading(false);
    } catch (error) {
      console.error("Error fetching data:", error);
      setLoading(false);
    }
  };

  // 3. Run this automatically when the page loads
  useEffect(() => {
    fetchData(); // Fetch immediately
    
    // Auto-refresh the data every 10 seconds!
    const interval = setInterval(fetchData, 10000);
    return () => clearInterval(interval);
  }, []);

  // 4. Build the visual HTML UI
  return (
    <div className="dashboard">
      <h1>🌡️ WeatherRep Dashboard</h1>
      
      {loading ? (
        <p>Loading data from AWS cloud...</p>
      ) : (
        <div className="card-container">
          {sensorData.map((reading) => (
            <div key={reading.id} className="card">
              <h3>{reading.device_id}</h3>
              <p><strong>Temp:</strong> {reading.temperature} °C</p>
              <p><strong>Humidity:</strong> {reading.humidity} %</p>
              <p className="time">Recorded: {new Date(reading.timestamp).toLocaleString()}</p>
            </div>
          ))}
        </div>
      )}
    </div>
  )
}

export default App