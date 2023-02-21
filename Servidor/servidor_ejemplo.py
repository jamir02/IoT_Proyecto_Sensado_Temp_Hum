from fastapi import FastAPI
import uvicorn
from datetime import datetime
from pydantic import BaseModel

app = FastAPI()


class Payload(BaseModel):
    data: int
    timestamp: datetime


class SensorDistancia(BaseModel):
    data: int


class TempHumSensor(BaseModel):
    temp: float
    hum: float
    timestamp: datetime

@app.get("/")
async def index():
    return {"message": "Hello World"}


@app.post("/sensor")
async def crea_dato_sensor(payload: Payload):
    print(f"Dato es {payload.data} y timestamp es {payload.timestamp}")

    return {"message": "Dato recibido"}


@app.post("/sensor/distancia")
async def crea_dato_sensor_distancia(payload: SensorDistancia):
    print(f"Dato es {payload.data}")

    return {"message": "Dato recibido"}


@app.post("/sensor/tempHumSensor")
async def crea_dato_sensor_temp_hum(payload: TempHumSensor):
    print(f"Temperatura es {payload.temp}, la humedad es {payload.hum} y el timestamp es {payload.timestamp}")
    with open("datos.csv", "r") as f:
        contenido = f.read()
    
    contenido += f"{payload.temp},{payload.hum},{payload.timestamp}\n"

    with open("datos.csv", "w+") as f:
        f.write(contenido)

    return {"message": "Dato recibido"}


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=5000)