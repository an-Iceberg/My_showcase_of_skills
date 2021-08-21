import { Request, Response } from "express";
import path from "path";

let contactPath = path.join(__dirname, '../views/contact.html');

export function contact(request: Request, response: Response) {
    response.sendFile(contactPath);
}
