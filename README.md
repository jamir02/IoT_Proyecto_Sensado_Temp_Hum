# IoT_Proyecto_Sensado_Temp_Hum

Este es un proyecto del curso de IoT de la diplomatura de Transformación Digital, donde se desarrolló un programa para el microcontrolador ESP-32 que consistía en generar el timestamp de las lecturas de los sensores de temperatura y humedad a partir de la conexión con un servidor NTP ("pool.ntp.org"). 


Dicho timestamp junto con las mediciones de la temperatura y humedad fueron enviadas a un servidor local, en el cual se utilizó la librería FastAPI para crear una API. Este servidor tenía el objetivo de recepcionar la información enviada por el ESP-32 y almacenar las lecturas de temperatura, humedad y el timestamp con el formato ISO en un archivo .csv. 


**Integrante:**

- Jamir Villarroel Segovia


