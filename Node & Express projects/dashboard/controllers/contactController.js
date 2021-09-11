"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.contact = void 0;
var path_1 = __importDefault(require("path"));
var contactPath = path_1.default.join(__dirname, '../views/contact.html');
function contact(request, response) {
    response.sendFile(contactPath);
}
exports.contact = contact;
