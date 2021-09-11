"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
var express_1 = __importDefault(require("express"));
var router_1 = require("../routers/router");
var express_handlebars_1 = __importDefault(require("express-handlebars"));
var server = express_1.default();
server.use(router_1.router);
server.use(express_1.default.static('public')); // Make sure to clear the path in the index.html file
// Setting up handlebars
server.set('viewDir', 'views');
server.engine('html', express_handlebars_1.default({ extname: 'html' }));
server.set('view engine', 'html');
var port = process.env.PORT || 5000;
server.listen(port, function () {
    console.log("Server listening on port " + port);
});
