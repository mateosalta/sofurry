#include <client.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

Client::Client(Config::Ptr config) :
    config_(config), cancelled_(false) {
}


void Client::get(const net::Uri::Path &path,
                 const net::Uri::QueryParameters &parameters, QJsonDocument &root) {
    // Create a new HTTP client
    auto client = http::make_client();

    // Start building the request configuration
    http::Request::Configuration configuration;

    // Build the URI from its components
    net::Uri uri = net::make_uri(config_->apiroot, path, parameters);
    configuration.uri = client->uri_to_string(uri);

    // Give out a user agent string
    configuration.header.add("User-Agent", config_->user_agent);

    // Build a HTTP request object from our configuration
    auto request = client->head(configuration);

    try {
        // Synchronously make the HTTP request
        // We bind the cancellable callback to #progress_report
        auto response = request->execute(
                    bind(&Client::progress_report, this, placeholders::_1));

        // Check that we got a sensible HTTP status code
        if (response.status != http::Status::ok) {
            throw domain_error(response.body);
        }
        // Parse the JSON from the response
        root = QJsonDocument::fromJson(response.body.c_str());

        // Open weather map API error code can either be a string or int
        QVariant cod = root.toVariant().toMap()["cod"];
        if ((cod.canConvert<QString>() && cod.toString() != "200")
                || (cod.canConvert<unsigned int>() && cod.toUInt() != 200)) {
            throw domain_error(root.toVariant().toMap()["message"].toString().toStdString());
        }
    } catch (net::Error &) {
    }
}

Client::ArtRes Client::art_daily() {
    QJsonDocument root;

    // Build a URI and get the contents
    // The fist parameter forms the path part of the URI.
    // The second parameter forms the CGI parameters.
    get( { "browse", "all", "art" }, { { "format", "json" }
         }, root);
    // https://api2.sofurry.com/browse/all/art?format=json

    ArtRes result;

    QVariantMap variant = root.toVariant().toMap();


    for (const QVariant &i : variant["items"].toList()) {
        QVariantMap item = i.toMap();


        // Extract the temperature data

        // Add a result to the weather list
        result.arts.emplace_back(
                    Art {
                        item["thumbnail"].toString().toStdString(),
                        item["title"].toString().toStdString(),
                        item["author"].toString().toStdString(),
                        item["authorID"].toString().toStdString(),
                        item["link"].toString().toStdString(),
                        item["tags"].toString().toStdString(),
                        item["description"].toString().toStdString(),

                    });








        /*  QJsonDocument root;

    // Build a URI and get the contents.
    // The fist parameter forms the path part of the URI.
    // The second parameter forms the CGI parameters.
    get( { "api", "similar"}, { { "k", key_sys }, { "q", query }, { "info", "1"}, {"limit", rest_sys }}, root);
    // https://www.tastekid.com/api/similar?k=147022-TasteKid-3H5727D0&q=ratatat

    // My “list of info” object (as seen in the corresponding header file)
    InfoRes result;

    QVariantMap variant = root.toVariant().toMap();
    QVariantMap Similar = variant["Similar"].toMap();
    for (const QVariant &i : Similar["Info"].toList()) {
        QVariantMap item = i.toMap();
        //cout << item["Name"].toString().toStdString();
        // We add each result to our list
        result.infos.emplace_back(
            Info {
                item["Name"].toString().toStdString(),
                item["Type"].toString().toStdString(),
                item["wTeaser"].toString().toStdString(),
                item["wUrl"].toString().toStdString(),
                item["yUrl"].toString().toStdString(),
                item["yID"].toString().toStdString(),
            }

        );
    }
    return result; */

    }

    return result;
}
Client::StorieRes Client::storie_daily() {
    QJsonDocument root;

    // Build a URI and get the contents
    // The fist parameter forms the path part of the URI.
    // The second parameter forms the CGI parameters.
    get( { "browse", "all", "stories" }, { { "format", "json" }
         }, root);
    // https://api2.sofurry.com/browse/all/stories?format=json

    StorieRes result;

    QVariantMap variant = root.toVariant().toMap();


    for (const QVariant &i : variant["items"].toList()) {
        QVariantMap item = i.toMap();


        // Extract the temperature data

        // Add a result to the weather list
        result.stories.emplace_back(
                    Storie {
                        item["thumbnail"].toString().toStdString(),
                        item["title"].toString().toStdString(),
                        item["author"].toString().toStdString(),
                        item["authorID"].toString().toStdString(),
                        item["link"].toString().toStdString(),
                        item["tags"].toString().toStdString(),
                        item["description"].toString().toStdString(),
                    });
    }

    return result;
}

Client::MusicRes Client::music_daily() {
    QJsonDocument root;

    // Build a URI and get the contents
    // The fist parameter forms the path part of the URI.
    // The second parameter forms the CGI parameters.
    get( { "browse", "all", "music" }, { { "format", "json" }
         }, root);
    // https://api2.sofurry.com/browse/all/photos?format=json
    MusicRes result;

    QVariantMap variant = root.toVariant().toMap();


    for (const QVariant &i : variant["items"].toList()) {
        QVariantMap item = i.toMap();


        // Extract the temperature data

        // Add a result to the weather list
        result.musics.emplace_back(
                    Music {
                        item["thumbnail"].toString().toStdString(),
                        item["title"].toString().toStdString(),
                        item["author"].toString().toStdString(),
                        item["authorID"].toString().toStdString(),
                        item["link"].toString().toStdString(),
                        item["tags"].toString().toStdString(),
                        item["description"].toString().toStdString(),
                    });
    }

    return result;
}

Client::PhotoRes Client::photo_daily() {
    QJsonDocument root;

    // Build a URI and get the contents
    // The fist parameter forms the path part of the URI.
    // The second parameter forms the CGI parameters.
    get( { "browse", "all", "photos" }, { { "format", "json" }
         }, root);
    // https://api2.sofurry.com/browse/all/photos?format=json

    PhotoRes result;

    QVariantMap variant = root.toVariant().toMap();


    for (const QVariant &i : variant["items"].toList()) {
        QVariantMap item = i.toMap();


        // Extract the temperature data

        // Add a result to the weather list
        result.photos.emplace_back(
                    Photo {
                        item["thumbnail"].toString().toStdString(),
                        item["title"].toString().toStdString(),
                        item["author"].toString().toStdString(),
                        item["authorID"].toString().toStdString(),
                        item["link"].toString().toStdString(),
                        item["tags"].toString().toStdString(),
                        item["description"].toString().toStdString(),
                    });
    }

    return result;
}

Client::JournalRes Client::journal_daily() {
    QJsonDocument root;

    // Build a URI and get the contents
    // The fist parameter forms the path part of the URI.
    // The second parameter forms the CGI parameters.
    get( { "browse", "all", "journals" }, { { "format", "json" }
         }, root);
    // https://api2.sofurry.com/browse/all/journals?format=json

    JournalRes result;

    QVariantMap variant = root.toVariant().toMap();


    for (const QVariant &i : variant["items"].toList()) {
        QVariantMap item = i.toMap();


        // Extract the temperature data

        // Add a result to the weather list
        result.journals.emplace_back(
                    Journal {
                        item["thumbnail"].toString().toStdString(),
                        item["title"].toString().toStdString(),
                        item["author"].toString().toStdString(),
                        item["authorID"].toString().toStdString(),
                        item["link"].toString().toStdString(),
                        item["tags"].toString().toStdString(),
                        item["description"].toString().toStdString(),
                    });
    }

    return result;
}

Client::SearchRes Client::search_daily(const std::string &query) {
    QJsonDocument root;


    //"searchType":"all","searchTerm":"wolf","data":{"entries":[{"

    /*
    StreamRes result;

    QVariantMap variant = root.toVariant().toMap();

    // Iterate through the stream data
    for (const QVariant &i : variant["streams"].toList()) {

        QVariantMap channel = i.toMap();
        QVariantMap previews = channel["preview"].toMap();

        std::string viewers = channel["viewers"].toString().toStdString();
        channel = channel["channel"].toMap();

    */


    // Build a URI and get the contents
    // The fist parameter forms the path part of the URI.
    // The second parameter forms the CGI parameters.
    get( { "browse", "search"}, { {"search", query}, { "format", "json" }
         }, root);
    // https://api2.sofurry.com/browse/search?search=dildo&format=json

    SearchRes result;

    QVariantMap variant = root.toVariant().toMap();

    for (const QVariant &i : variant["data"].toList()) {
        QVariantMap item = i.toMap();


   /*QVariantMap variant = root.toVariant().toMap();
    QVariantMap Similar = variant["Similar"].toMap();
    for (const QVariant &i : Similar["Results"].toList()) {
        QVariantMap item = i.toMap();
        result.results.emplace_back(
            Result { */
        QVariantMap data = item["entries"].toMap();


    for (const QVariant &i : data["entries"].toList()) {
        QVariantMap item = i.toMap();
        //entries = entries["entries"].toMap();


 /* broke stuff/       result.searchs.emplace_back(
                    Search {
                        Entries {
                        item["pageid"].toString().toStdString(),
                        item["ns"].toString().toStdString(),
                        item["title"].toString().toStdString(),
                        item["pageimage"].toString().toStdString()
                    });

*/




        // Extract the temperature data

        // Add a result to the weather list
}
    }

    return result;
}

/*   QVariantMap variant = root.toVariant().toMap();


    QVariantMap responseData = variant["responseData"].toMap();
    // Iterate
    for (const QVariant &i : responseData["entries"].toList()) {


    // Read out the city we found
    QVariantMap aquery = variant["query"].toMap();
    //QVariantMap pages = query["pages"].toMap();

   // QVariantMap thumbnail = pages["thumbnail"].toMap();
    for (const QVariant &i : aquery["pages"].toList()) {
        QVariantMap item = i.toMap();

        result.weather.emplace_back(
                    Weather {
                        item["pageid"].toString().toStdString(),
                        item["ns"].toString().toStdString(),
                        item["title"].toString().toStdString(),
                        item["pageimage"].toString().toStdString()
                    });
        QVariantMap thumb = item["thumbnail"].toMap();



    result.thumbnail.emplace_back(
                Thumbnail {
                    thumb["source"].toString().toStdString(),
                });
}*/

http::Request::Progress::Next Client::progress_report(
        const http::Request::Progress&) {

    return cancelled_ ?
                http::Request::Progress::Next::abort_operation :
                http::Request::Progress::Next::continue_operation;
}

void Client::cancel() {
    cancelled_ = true;
}

Client::Config::Ptr Client::config() {
    return config_;
}

