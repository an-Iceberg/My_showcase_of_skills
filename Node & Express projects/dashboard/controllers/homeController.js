"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    Object.defineProperty(o, k2, { enumerable: true, get: function() { return m[k]; } });
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.home = void 0;
// import path from "path";
var nodeFetch = __importStar(require("node-fetch"));
// import Handlebars from "handlebars";
// let homePath = path.join(__dirname, '../views/index.html');
function home(request, responseHome) {
    nodeFetch.default("https://newsapi.org/v2/everything?q=apple&from=2021-08-18&to=2021-08-18&sortBy=popularity&apiKey=42ba0cf028bb4eba81b1274db27ccfc2")
        // fetching the news from the source in a json file
        .then(function (response) {
        return response.json();
    })
        // baking the relevant information into the html file
        .then(function (news) {
        // let news = JSON.parse(news);
        responseHome.render('index', {
            title0: news.articles[0].title,
            author0: news.articles[0].author,
            publishedAt0: news.articles[0].publishedAt,
            url0: news.articles[0].url,
            urlToImage0: news.articles[0].urlToImage,
            description0: news.articles[0].description,
            title1: news.articles[1].title,
            author1: news.articles[1].author,
            publishedAt1: news.articles[1].publishedAt,
            url1: news.articles[1].url,
            urlToImage1: news.articles[1].urlToImage,
            description1: news.articles[1].description,
            title2: news.articles[2].title,
            author2: news.articles[2].author,
            publishedAt2: news.articles[2].publishedAt,
            url2: news.articles[2].url,
            urlToImage2: news.articles[2].urlToImage,
            description2: news.articles[2].description,
            title3: news.articles[3].title,
            author3: news.articles[3].author,
            publishedAt3: news.articles[3].publishedAt,
            url3: news.articles[3].url,
            urlToImage3: news.articles[3].urlToImage,
            description3: news.articles[3].description,
            title4: news.articles[4].title,
            author4: news.articles[4].author,
            publishedAt4: news.articles[4].publishedAt,
            url4: news.articles[4].url,
            urlToImage4: news.articles[4].urlToImage,
            description4: news.articles[4].description,
            title5: news.articles[5].title,
            author5: news.articles[5].author,
            publishedAt5: news.articles[5].publishedAt,
            url5: news.articles[5].url,
            urlToImage5: news.articles[5].urlToImage,
            description5: news.articles[5].description,
            title6: news.articles[6].title,
            author6: news.articles[6].author,
            publishedAt6: news.articles[6].publishedAt,
            url6: news.articles[6].url,
            urlToImage6: news.articles[6].urlToImage,
            description6: news.articles[6].description,
            title7: news.articles[7].title,
            author7: news.articles[7].author,
            publishedAt7: news.articles[7].publishedAt,
            url7: news.articles[7].url,
            urlToImage7: news.articles[7].urlToImage,
            description7: news.articles[7].description,
            title8: news.articles[8].title,
            author8: news.articles[8].author,
            publishedAt8: news.articles[8].publishedAt,
            url8: news.articles[8].url,
            urlToImage8: news.articles[8].urlToImage,
            description8: news.articles[8].description,
            layout: false
        });
    })
        .catch(function (error) {
        console.log(error);
    });
    // response.render('index', {
    //     title1: articles,
    //     layout: false
    // });
    // response.sendFile(homePath);
}
exports.home = home;
