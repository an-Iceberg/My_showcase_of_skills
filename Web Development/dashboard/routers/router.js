"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.router = void 0;
var express_1 = __importDefault(require("express"));
var homeController_1 = require("../controllers/homeController");
var contactController_1 = require("../controllers/contactController");
var aboutController_1 = require("../controllers/aboutController");
exports.router = express_1.default();
exports.router.use(express_1.default.urlencoded({ extended: true }));
exports.router.get('/', homeController_1.home);
exports.router.get('/contact', contactController_1.contact);
exports.router.get('/about', aboutController_1.about);
