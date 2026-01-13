#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}

    //DESCTROCTOR FOR DJLIBRARYSERVICE
    DJLibraryService::~DJLibraryService(){
    for(AudioTrack* track: library){ //deleting the tracks
        delete track;
    }
    for(size_t i=0;i<library.size();i++){ //removing the pointers
        library[i]=nullptr;
    }
    
}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for(size_t i=0;i<library_tracks.size();i++){
        SessionConfig::TrackInfo track = library_tracks[i];
        std::string format = track.type; //WAV or MP3
        if(format=="WAV"){
            //creating a new WAVTrack and storing it in the library
            WAVTrack* new_track = new WAVTrack(track.title,track.artists,track.duration_seconds,track.bpm,track.extra_param1,track.extra_param2);
            library.push_back(new_track);
        }
        else{
            //creating a new MP3Track and storing it in the library
            MP3Track* new_track = new MP3Track(track.title,track.artists,track.duration_seconds,track.bpm,track.extra_param1,track.extra_param2);
            library.push_back(new_track);
        }
    }
    std::cout << "[INFO] Track library built: "<< library.size() <<" tracks loaded\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    //search in playlist for track by title
    return playlist.find_track(track_title); //using the find_track function of playlist
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";
    playlist = Playlist(playlist_name); //creating the playlist
    for(size_t index: track_indices){
        if(index<=0||index>library.size()){ //checking that the index is within library bounds
            std::cout << "[WARNING] Invalid track index: " << index <<"\n";
            continue; //skips the current index
        }
        else{
            AudioTrack* track = library[index-1]->clone().release(); //cloning the track polimorphically
            if(!track){
                std::cout << "[ERROR] Track clone failed for index: " << index << "\n";
                continue;
            }
            else{
                //adding the track to the playlist
                track->load();
                track->analyze_beatgrid();
                playlist.add_track(track);
            }
        }
    }
    std::cout << "[INFO] Playlist loaded: "<< playlist.get_name() << " (" << playlist.get_track_count() << " tracks)\n";
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<AudioTrack*> tracks = playlist.getTracks(); //getting the tracks to iterate through
    std::vector<std::string> titles_list; //creating an empty string vector
    for(AudioTrack* track: tracks){
        titles_list.push_back(track->get_title()); //adding the titles to the vector
    }
    return titles_list;
}
