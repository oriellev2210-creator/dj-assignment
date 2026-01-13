#include "MP3Track.h"
#include <iostream>
#include <cmath>
#include <algorithm>

MP3Track::MP3Track(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int bitrate, bool has_tags)
    : AudioTrack(title, artists, duration, bpm), bitrate(bitrate), has_id3_tags(has_tags) {

    std::cout << "MP3Track created: " << bitrate << " kbps" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void MP3Track::load() {
    std::cout << "[MP3Track::load] Loading MP3: \"" << title
              << "\" at " << bitrate << " kbps...\n";
    // TODO: Implement MP3 loading with format-specific operations
    if(has_id3_tags){
        std::cout << "  → Processing ID3 metadata (artist info, album art, etc.)...\n";
    }
    else{
        std::cout << "  → No ID3 tags found.\n";
    }
    std::cout << "  → Decoding MP3 frames...\n";
    std::cout << "  → Load complete.\n";
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    
}

void MP3Track::analyze_beatgrid() {
     std::cout << "[MP3Track::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement MP3-specific beat detection analysis
    int beats_estimated = (int)((duration_seconds/60.0)*bpm);
    double precision_factor = bitrate / 320.0;
    std::cout << "  → Estimated beats: " << beats_estimated << "  → Compression precision factor: " << precision_factor << "\n";
    // NOTE: Use exactly 2 spaces before each arrow (→) character

}

double MP3Track::get_quality_score() const {
    // TODO: Implement comprehensive quality scoring
    // NOTE: This method does NOT print anything
    double base_score = (bitrate / 320.0) * 100.0 ;
    if (has_id3_tags){
        base_score +=5;
    }
    if(bitrate < 128){
        base_score-=10;
    }
    if (base_score < 0){
        base_score = 0.0;
    }
    if (base_score > 100){
        base_score = 100.0;
    }
    return base_score; // Replace with your implementation
}

PointerWrapper<AudioTrack> MP3Track::clone() const {
    // TODO: Implement polymorphic cloning
    MP3Track* copy = new MP3Track(*this); //creating a deep copy of our MP3Track using the copy constructor and saving its address
    return PointerWrapper<AudioTrack>(copy); // wrapping the pointer to the copy
}