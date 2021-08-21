import { Request, Response } from "express";
// import path from "path";
import * as nodeFetch from "node-fetch";
// import Handlebars from "handlebars";

// let homePath = path.join(__dirname, '../views/index.html');

export function home(request: Request, responseHome: Response) {

    nodeFetch.default("https://newsapi.org/v2/everything?q=apple&from=2021-08-18&to=2021-08-18&sortBy=popularity&apiKey=42ba0cf028bb4eba81b1274db27ccfc2")
        // fetching the news from the source in a json file
        .then((response: nodeFetch.Response) => {
            return response.json();
        })
        // baking the relevant information into the html file
        .then((news) => {
            // let news = JSON.parse(news);
            responseHome.render('index', {
                title0: news.articles[0].title,
                author0: news.articles[0].author,
                publishedAt0: news.articles[0].publishedAt,
                urlToImage0: news.articles[0].urlToImage,
                description0: news.articles[0].description,

                title1: news.articles[1].title,
                author1: news.articles[1].author,
                publishedAt1: news.articles[1].publishedAt,
                urlToImage1: news.articles[1].urlToImage,
                description1: news.articles[1].description,

                title2: news.articles[2].title,
                author2: news.articles[2].author,
                publishedAt2: news.articles[2].publishedAt,
                urlToImage2: news.articles[2].urlToImage,
                description2: news.articles[2].description,

                layout: false
            })
        })
        .catch((error) => {
            console.log(error);
        })

    // response.render('index', {
    //     title1: articles,
    //     layout: false
    // });

    // response.sendFile(homePath);
}
