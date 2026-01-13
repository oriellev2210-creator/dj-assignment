#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    //checking if track with the same title exists in cache
    if(cache.contains(track.get_title())){
        //HIT case
        cache.get(track.get_title());
        return 1;
    }
    else{
        //MISS case - inserting the track manually
        AudioTrack* cloned_track = track.clone().release();
        if(!cloned_track){
            std::cout << "[ERROR] Track: " << track.get_title() << " failed to clone\n";
            return 0;
        }
        else{
            cloned_track->load();
            cloned_track->analyze_beatgrid();
            PointerWrapper<AudioTrack> wrapped_clone(cloned_track);
            bool evicted = cache.put(std::move(wrapped_clone));
            if(evicted){
                return -1;
            }
            else{
                return 0;
            }
        }
    }
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return cache.get(track_title);
}
