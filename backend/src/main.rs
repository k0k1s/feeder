use actix_web::{web, App, HttpServer, Responder, HttpResponse};
use reqwest::Client;
use select::document::Document;
use select::predicate::{Name, Class, Predicate};
use serde::Serialize;
use std::error::Error;

#[derive(Serialize, Clone)]
struct NewsItem {
    title: String,
    link: String,
}

async fn fetch_news() -> Result<Vec<NewsItem>, Box<dyn Error>> {
    let url = "https://thehackernews.com/";
    let client = Client::new();
    let res = client.get(url).send().await?.text().await?;
    
    let document = Document::from(res.as_str());
    let mut news_items = Vec::new();

    for node in document.find(Name("h2").and(Class("post-title"))) {
        let title = node.text();
        let link = node.find(Name("a")).next().ok_or("Link not found")?.attr("href").unwrap_or("").to_string();
        
        news_items.push(NewsItem { title, link });
    }

    Ok(news_items)
}

async fn get_news() -> impl Responder {
    match fetch_news().await {
        Ok(news) => HttpResponse::Ok().json(news),
        Err(e) => HttpResponse::InternalServerError().body(format!("Error fetching news: {}", e)),
    }
}

#[tokio::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| {
        App::new()
            .route("/news", web::get().to(get_news))
    })
    .bind("127.0.0.1:8080")?
    .run()
    .await
}
