#include <query.h>
#include <localization.h>

#include <unity/scopes/Annotation.h>
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace sc = unity::scopes;

using namespace std;


const static string FORECAST_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "carousel",
        "card-layout": "vertical",
        "card-size": "large",
        "overlay": true
        },
        "components": {
        "title": "title",
        "art" : {
        "field": "art",
        "aspect-ratio": ".875"

        },
        "subtitle": "subtitle"
        }
        }
        )";

const static string STORIE_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "vertical-journal",
        "card-layout": "horizontal",
        "card-size": "large",
        "overlay": false

        },
        "components": {
        "title": "title",
        "mascot" : {
        "field": "art",
        "aspect-ratio": 1
        },
        "subtitle": "subtitle",
        "summary": "summary"
        }
        }
        )";

const static string JOURNAL_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "vertical-journal",
        "card-layout": "vertical",
        "card-size": "medium",
        "overlay": false

        },
        "components": {
        "title": "title",
        "art" : {
        "field": "art",
        "aspect-ratio": 1
        },
        "subtitle": "subtitle",
        "summary": "summary"
        }
        }
        )";

const static string MUSIC_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "horizontal-list",
        "card-layout": "horizontal",
        "card-size": "small",
        "overlay": false

        },
        "components": {
        "title": "title",
        "art" : {
        "field": "art"
        },
        "subtitle": "subtitle"

        }
        }
        )";
const static string PHOTO_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "grid",
        "card-layout": "vertical",
        "card-size": "small",
        "overlay": false

        },
        "components": {
        "title": "title",
        "art" : {
        "field": "art"
        },
        "subtitle": "subtitle"

        }
        }
        )";

const static string SEARCHS_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "grid",
        "card-layout": "vertical",
        "card-size": "small"
        },
        "components": {
        "title": "title",
        "art" : {
        "field": "art"
        },
        "subtitle": "subtitle"
        }
        }
        )";
Query::Query(const sc::CannedQuery &query, const sc::SearchMetadata &metadata,
             Client::Config::Ptr config) :
    sc::SearchQueryBase(query, metadata), client_(config) {
}

void Query::cancelled() {
    client_.cancel();
}


void Query::run(sc::SearchReplyProxy const& reply) {
    try {
        // Start by getting information about the query
        const sc::CannedQuery &query(sc::SearchQueryBase::query());

        // Get the query string
        string query_string = query.query_string();



        // Build up the description for the city



        Client::ArtRes artlist ;
        if (query_string.empty()) {
            // If there is no search string, get the forecast for London
            artlist = client_.art_daily();
        } else {
            // otherwise, get the forecast for the search string
        }

        // Register a category for the forecast
        auto art_cat = reply->register_category("art", "Art", "",
                                                     sc::CategoryRenderer(FORECAST_TEMPLATE));

        // For each of the forecast days
        for (const auto &Art : artlist.arts) {
            // Create a result
            sc::CategorisedResult res(art_cat);

            // We must have a URI
            res.set_uri(Art.link);

            // Build the description for the result

            // Set the rest of the attributes
            res.set_art(Art.thumbnail);
            res["title"] = Art.title;
            res["subtitle"] = Art.author;
            res["description"] = "<strong>Tags:</strong> " + Art.tags + "<br><br>" + Art.description;
            res["link"] = Art.link;

            // Push the result
            if (!reply->push(res)) {
                // If we fail to push, it means the query has been cancelled.
                // So don't continue;
                return;
            }
        }

        Client::MusicRes musiclist;
        if (query_string.empty()) {
            // If there is no search string, get the forecast for London
            musiclist = client_.music_daily();
        } else {
            // otherwise, get the forecast for the search string

        }

        // Register a category for the forecast
        auto music_cat = reply->register_category("music", "Music", "",
                                                     sc::CategoryRenderer(MUSIC_TEMPLATE));

        // For each of the forecast days
        for (const auto &Music : musiclist.musics) {
            // Create a result
            sc::CategorisedResult res(music_cat);

            // We must have a URI
            res.set_uri(Music.link);

            // Build the description for the result

            // Set the rest of the attributes
            res.set_art(Music.thumbnail);
            res["title"] = Music.title;
            res["subtitle"] = Music.author;
            res["link"] = Music.link;
            res["description"] = "<strong>Tags:</strong> " + Music.tags + "<br><br>" + Music.description;
            // Push the result
            if (!reply->push(res)) {
                // If we fail to push, it means the query has been cancelled.
                // So don't continue;
                return;
            }

        }


        Client::PhotoRes photolist;
        if (query_string.empty()) {
            // If there is no search string, get the forecast for London
            photolist = client_.photo_daily();
        } else {
            // otherwise, get the forecast for the search string

        }

        // Register a category for the forecast
        auto photo_cat = reply->register_category("photo", "Photos", "",
                                                     sc::CategoryRenderer(PHOTO_TEMPLATE));

        // For each of the forecast days
        for (const auto &Photo : photolist.photos) {
            // Create a result
            sc::CategorisedResult res(photo_cat);

            // We must have a URI
            res.set_uri(Photo.link);

            // Build the description for the result

            // Set the rest of the attributes
            res.set_art(Photo.thumbnail);
            res["title"] = Photo.title;
            res["subtitle"] = Photo.author;
            res["summary"] =   Photo.description + "<br>" + Photo.tags;
            res["link"] = Photo.link;
            res["description"] = "<strong>Tags:</strong> " + Photo.tags + "<br><br>" + Photo.description;
            // Push the result
            if (!reply->push(res)) {
                // If we fail to push, it means the query has been cancelled.
                // So don't continue;
                return;
            }

        }



        Client::StorieRes storielist;
        if (query_string.empty()) {
            // If there is no search string, get the forecast for London
            storielist = client_.storie_daily();
        } else {
            // otherwise, get the forecast for the search string

        }

        // Register a category for the forecast
        auto storie_cat = reply->register_category("storie", "Stories", "",
                                                     sc::CategoryRenderer(STORIE_TEMPLATE));

        // For each of the forecast days
        for (const auto &Storie : storielist.stories) {
            // Create a result
            sc::CategorisedResult res(storie_cat);

            // We must have a URI
            res.set_uri(Storie.link);

            // Build the description for the result

            // Set the rest of the attributes
            res.set_art(Storie.thumbnail);
            res["title"] = Storie.title;
            res["subtitle"] = Storie.author;
            res["summary"] =   Storie.description + "<br>" + Storie.tags;
            res["link"] = Storie.link;
            res["description"] = "<strong>Tags:</strong> " + Storie.tags + "<br><br>" + Storie.description;
            // Push the result
            if (!reply->push(res)) {
                // If we fail to push, it means the query has been cancelled.
                // So don't continue;
                return;
            }

        }

        Client::JournalRes journallist;
        if (query_string.empty()) {
            // If there is no search string, get the forecast for London
            journallist = client_.journal_daily();
        } else {
            // otherwise, get the forecast for the search string

        }

        // Register a category for the forecast
        auto journal_cat = reply->register_category("journal", "Journals", "",
                                                     sc::CategoryRenderer(JOURNAL_TEMPLATE));

        // For each of the forecast days
        for (const auto &Journal : journallist.journals) {
            // Create a result
            sc::CategorisedResult res(journal_cat);

            // We must have a URI
            res.set_uri(Journal.link);

            // Build the description for the result

            // Set the rest of the attributes
            res.set_art(Journal.thumbnail);
            res["title"] = Journal.title;
            res["subtitle"] = Journal.author;
            res["summary"] =   Journal.description + "<br>" + Journal.tags;
            res["link"] = Journal.link;
            res["description"] = "<strong>Tags:</strong> " + Journal.tags + "<br><br>" + Journal.description;
            // Push the result
            if (!reply->push(res)) {
                // If we fail to push, it means the query has been cancelled.
                // So don't continue;
                return;
            }

        }




        Client::SearchRes searchlist;
        if (query_string.empty()) {
            // If there is no search string, get the forecast for London
            searchlist = client_.search_daily("wolf");
        } else {
            // otherwise, get the forecast for the search string
            searchlist = client_.search_daily(query_string);

        }

        // Register a category for the forecast
        auto search_cat = reply->register_category("search", "Search", "",
                                                     sc::CategoryRenderer(SEARCHS_TEMPLATE));

        // For each of the forecast days
        
        /*for (const auto &thumbnail : forecast.thumbnail) {
            // Create a result
          //  sc::CategorisedResult res(forecast_cat);
           // res.set_uri(thumbnail.source);
          //  res["actions"] = thumbnail.source;
            sc::CategorisedResult res(forecast_cat);

            res.set_uri(thumbnail.source);


            for (const auto &weather : forecast.weather) { */
        
        
        for (const auto &Search : searchlist.searchs) {
            // Create a result
            sc::CategorisedResult res(search_cat);

            // We must have a URI
            res.set_uri(Search.title);

            // Build the description for the result

            // Set the rest of the attributes
            res.set_art(Search.thumbnail);
            res["title"] = Search.title;
            res["subtitle"] = Search.author;
            //res["description"] = Search.description;
            // Push the result
            if (!reply->push(res)) {
                // If we fail to push, it means the query has been cancelled.
                // So don't continue;
                return;
            }

        }

    } catch (domain_error &e) {
        // Handle exceptions being thrown by the client API
        cerr << e.what() << endl;
        reply->error(current_exception());
    }
}

