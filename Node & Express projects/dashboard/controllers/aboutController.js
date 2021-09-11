"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.about = void 0;
var path_1 = __importDefault(require("path"));
var aboutPath = path_1.default.join(__dirname, '../views/about.html');
function about(request, response) {
    response.sendFile(aboutPath);
}
exports.about = about;
