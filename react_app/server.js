require("dotenv").config();
const express = require("express");
const mongoose = require("mongoose");
const cors = require("cors");
const bodyParser = require("body-parser");

const app = express();
const PORT = 4000;

// Middleware
app.use(cors());
app.use(bodyParser.json());

// Connect to MongoDB Atlas (Using Environment Variable)
mongoose.connect(
  "mongodb+srv://zobia:IuUQ7qwSI3A4mP89@cluster0.t8iy8ur.mongodb.net/sensorDB?retryWrites=true&w=majority&appName=Cluster0",
  {
    useNewUrlParser: true,
    useUnifiedTopology: true,
  }
)
.then(() => console.log("✅ MongoDB Atlas connected successfully"))
.catch(err => {
  console.error("❌ MongoDB connection error:", err);
  process.exit(1); // Stop the server if DB connection fails
});


// Define Schema
const sensorSchema = new mongoose.Schema({
  temperature: { type: Number, required: true },
  humidity: { type: Number, required: true },
  timestamp: { type: Date, default: Date.now }
});

const SensorData = mongoose.model("SensorData", sensorSchema);

// Root Route (Check API Status)
app.get("/", (req, res) => {
  res.send("🚀 Sensor API is running...");
});

// ✅ API to Store Sensor Data
app.post("/api/sensor", async (req, res) => {
  try {
    const { temperature, humidity } = req.body;
    if (temperature === undefined || humidity === undefined) {
      return res.status(400).json({ error: "Missing data fields" });
    }

    const newData = new SensorData({ temperature, humidity });
    await newData.save();
    
    res.status(201).json({ message: "✅ Data saved successfully" });
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});

// ✅ API to Fetch All Sensor Data
app.get("/api/sensor", async (req, res) => {
  try {
    const data = await SensorData.find().sort({ timestamp: -1 }); // Fetch latest data first
    res.json(data);
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});

// Start Server
app.listen(PORT, () => {
  console.log(`🚀 Server running on http://localhost:${PORT}`);
});
