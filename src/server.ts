import express, { Application } from "express";
import socketIO, { Server as SocketIOServer } from "socket.io";
import { createServer, Server as HTTPServer } from "http";
import path from 'path'

export class Server {
    private httpServer: HTTPServer;
    private app: Application;
    private io: SocketIOServer;
    private activeSockets: string[] = [];

    private readonly DEFAULT_PORT = 5000;

    constructor() {
        this.app = express();
        this.httpServer = createServer(this.app);
        this.io = new socketIO.Server(this.httpServer);

        this.app.use(express.static(path.join(__dirname, "../public")));
        this.handleSocketConnection();
    }

    private handleSocketConnection(): void {
        this.io.on("connection", socket => {
            console.log("Socket connected")
            const existingSocket = this.activeSockets.find(
                sock => sock === socket.id
            );

            if (!existingSocket) {
                this.activeSockets.push(socket.id);

                socket.emit("update-user-list", {
                    users: this.activeSockets.filter(
                        sock => sock !== socket.id
                    )
                });

                socket.broadcast.emit("update-user-list", {
                    users: [socket.id]
                });
            }

            socket.on("disconnect", () => {
                this.activeSockets = this.activeSockets.filter(
                    sock => sock !== socket.id
                );
                socket.broadcast.emit("remove-user", {
                    socketId: socket.id
                });
            });
        })
    }

    public listen(callback: (port: number) => void): void {
        this.httpServer.listen(this.DEFAULT_PORT, () =>
            callback(this.DEFAULT_PORT)
        );
    }
}