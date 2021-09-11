import { Request, Response } from "express";
import path from "path";

let aboutPath = path.join(__dirname, '../views/about.html');

export function about(request: Request, response: Response) {
    response.sendFile(aboutPath);
}
