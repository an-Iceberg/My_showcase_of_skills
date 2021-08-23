import express from "express";
import { router } from "../routers/router";
import expressHandlebars from "express-handlebars";

const server = express();

server.use(router);
server.use(express.static('public')); // Make sure to clear the path in the index.html file

// Setting up handlebars
server.set('viewDir', 'views');
server.engine('html', expressHandlebars({ extname: 'html' }));
server.set('view engine', 'html');

let port = process.env.PORT || 5000;

server.listen(port, () => {
    console.log(`Server listening on port ${port}`);
})
