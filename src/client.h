#ifndef CLIENT_H_
#define CLIENT_H_

#include <atomic>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <core/net/http/request.h>
#include <core/net/uri.h>

#include <QJsonDocument>

/**
 * Provide a nice way to access the HTTP API.
 *
 * We don't want our scope's code to be mixed together with HTTP and JSON handling.
 */
class Client {
public:

    /**
     * Client configuration
     */
    struct Config {
        typedef std::shared_ptr<Config> Ptr;

        // The root of all API request URLs
        std::string apiroot { "https://api2.sofurry.com/" };

        // The custom HTTP user agent string for this library
        std::string user_agent { "example-network-scope 0.1; (foo)" };
    };



    /**
     * Weather information for a day.
     */
    struct Art {
        std::string thumbnail;
        std::string title;
        std::string author;
        std::string authorID;
        std::string link;
        std::string tags;
        std::string description;
    };

    struct Storie {
        std::string thumbnail;
        std::string title;
        std::string author;
        std::string authorID;
        std::string link;
        std::string tags;
        std::string description;
    };

    struct Music {
        std::string thumbnail;
        std::string title;
        std::string author;
        std::string authorID;
        std::string link;
        std::string tags;
        std::string description;
    };

    struct Photo {
        std::string thumbnail;
        std::string title;
        std::string author;
        std::string authorID;
        std::string link;
        std::string tags;
        std::string description;
    };
    struct Journal {
        std::string thumbnail;
        std::string title;
        std::string author;
        std::string authorID;
        std::string link;
        std::string tags;
        std::string description;
    };

    struct Search {
        std::string thumbnail;
        std::string title;
        std::string author;
        std::string authorID;
    };

    struct Entrie {
        std::string thumbnail;
        std::string title;
        std::string author;
        std::string authorID;
    };

    /**
     * A list of weather information
     */
    typedef std::deque<Art> ArtList;
    typedef std::deque<Storie> StorieList;
    typedef std::deque<Music> MusicList;
    typedef std::deque<Photo> PhotoList;
    typedef std::deque<Journal> JournalList;
    typedef std::deque<Entrie> EntrieList;

    typedef std::deque<Search> SearchList;


    /**
     * Weather information about the current day
     */

    /**
     * Forecast information about a city
     */
    struct ArtRes {
        ArtList arts;
    };
    struct StorieRes {
        StorieList stories;
    };
    struct MusicRes {
        MusicList musics;
    };
    struct PhotoRes {
        PhotoList photos;
    };
    struct JournalRes {
        JournalList journals;
    };
    struct SearchRes {
        SearchList searchs;
        EntrieList entries;
    };

    Client(Config::Ptr config);

    virtual ~Client() = default;

    /**
     * Get the current weather for the specified location
     */

    /**
     * Get the weather forecast for the specified location and duration
     */
    virtual ArtRes art_daily();
    virtual StorieRes storie_daily();
    virtual MusicRes music_daily();
    virtual PhotoRes photo_daily();
    virtual JournalRes journal_daily();
    virtual SearchRes search_daily(const std::string &query);


    /**
     * Cancel any pending queries (this method can be called from a different thread)
     */
    virtual void cancel();

    virtual Config::Ptr config();

protected:
    void get(const core::net::Uri::Path &path,
             const core::net::Uri::QueryParameters &parameters,
             QJsonDocument &root);
    /**
     * Progress callback that allows the query to cancel pending HTTP requests.
     */
    core::net::http::Request::Progress::Next progress_report(
            const core::net::http::Request::Progress& progress);

    /**
     * Hang onto the configuration information
     */
    Config::Ptr config_;

    /**
     * Thread-safe cancelled flag
     */
    std::atomic<bool> cancelled_;
};

#endif // CLIENT_H_

