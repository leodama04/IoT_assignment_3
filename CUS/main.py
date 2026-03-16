from fastapi import FastAPI, WebSocket
from fastapi.staticfiles import StaticFiles
from websocketConnection import ConnectionManager
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse
from model import State
import uvicorn
import logging

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s"
)

app = FastAPI()

app.mount("/static", StaticFiles(directory="../DBS"), name="static")


app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

state = State()
manager = ConnectionManager(state)

@app.get("/")
async def get():
    return FileResponse("../DBS/index.html")

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await manager.connect(websocket)
    
if __name__ == "__main__":
    uvicorn.run(app, host="localhost", port=8000)