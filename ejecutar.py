import subprocess
import random
import re

semilla = random.randint(40, 200)

def ejecutar_juego(i):
    semilla = random.randint(40, 200)
    comando = f"./Game Dummy Dummy Dummy Presi -s {semilla} -i default.cnf -o default.res{i}"
    resultado = subprocess.run(comando, shell=True, capture_output=True, text=True)
    return resultado.stderr

def obtener_puntuaciones_jugadores(salida):
    patron_puntuacion = r"player (.*) got score (\d+)"
    puntuaciones = re.findall(patron_puntuacion, salida, re.MULTILINE)
    return puntuaciones

def procesar_puntuaciones(puntuaciones):
    estadisticas = {}
    max_puntuacion = max(int(p[1]) for p in puntuaciones)
    for jugador, puntuacion in puntuaciones:
        if jugador not in estadisticas:
            estadisticas[jugador] = {"puntuaciones": [], "victorias": 0, "diferencias": []}
        puntuacion_int = int(puntuacion)
        estadisticas[jugador]["puntuaciones"].append(puntuacion_int)
        if puntuacion_int == max_puntuacion:
            estadisticas[jugador]["victorias"] += 1
            print(f"Jugador {jugador} ha ganado con {puntuacion_int} puntos")
            if jugador == "Dummy":
                with open("semillas_fallidas.txt", 'a') as archivo:
                    archivo.write(str(semilla)+'\n')
        else: 
            estadisticas[jugador]["diferencias"].append(max_puntuacion - puntuacion_int)

    return estadisticas


iteraciones = int(input("Numero de iteraciones: \n"))
estadisticas_totales = {}

for i in range(iteraciones):
    salida = ejecutar_juego(i)
    puntuaciones = obtener_puntuaciones_jugadores(salida)
    estadisticas = procesar_puntuaciones(puntuaciones)
    
    for jugador in estadisticas:
        if jugador not in estadisticas_totales:
            estadisticas_totales[jugador] = {"puntuaciones": [], "victorias": 0, "diferencias": []}
        estadisticas_totales[jugador]["puntuaciones"].extend(estadisticas[jugador]["puntuaciones"])
        estadisticas_totales[jugador]["victorias"] += estadisticas[jugador]["victorias"]
        estadisticas_totales[jugador]["diferencias"].extend(estadisticas[jugador]["diferencias"])

for jugador, datos in estadisticas_totales.items():
    media_puntuacion = sum(datos["puntuaciones"]) / len(datos["puntuaciones"]) if datos["puntuaciones"] else 0
    media_diferencia = sum(datos["diferencias"]) / len(datos["diferencias"]) if datos["diferencias"] else 0
    porcentaje_victorias = datos["victorias"] / iteraciones * 100

    print(f"Jugador {jugador}:")
    print(f"  Media de puntuación: {media_puntuacion}")
    print(f"  Total de victorias: {datos['victorias']} ({porcentaje_victorias}%)")
    print(f"  Media de diferencia de puntuación en derrotas: {media_diferencia}\n")
