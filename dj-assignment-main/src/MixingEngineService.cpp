#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
//CONSTRUCTOR
MixingEngineService::MixingEngineService()
    : decks(), active_deck(0), auto_sync(false), bpm_tolerance(0)
{
    decks[0]=nullptr;
    decks[1]=nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
//DESTRUCTOR
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks...\n";
    for(size_t i=0;i<=1;i++){
        if(decks[i]){
            delete decks[i];
            decks[i]=nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===\n";
    if(!decks[0] && !decks[1]){ //checking if this is the first track of the party 
        decks[0]=track.clone().release();
        if(!decks[0]){
            std::cout << "[ERROR] Track: "<< track.get_title() << " failed to clone\n";
            return -1;
        }
        std::cout << "[Deck Switch] Target deck: 0\n";
        decks[0]->load();
        decks[0]->analyze_beatgrid();
        std::cout << "[Load Complete] '" << decks[0]->get_title() << "' is now loaded on deck " << active_deck << "\n";
        std::cout << "[Active Deck] Switched to deck 0\n";
        return 0;
    }
    else{
        PointerWrapper<AudioTrack> new_track = track.clone(); //cloning the track for loading
        if(!new_track){
            std::cout << "[ERROR] Track: "<< track.get_title() << " failed to clone\n";
            return -1;
        }
        size_t target = 1-active_deck; //calculating the next deck to load to
        std::cout << "[Deck Switch] Target deck: " << target << "\n";
        if(decks[target]){ //deleting the track that currently occupies the target deck, to make room for the new track
            delete decks[target];
            decks[target]=nullptr;
        }
        //performing track preperation
        new_track->load();
        new_track->analyze_beatgrid();
        if(auto_sync && !can_mix_tracks(new_track)){ //checking if we can't mix the new track with the current one
            sync_bpm(new_track); //synchroniziing the track's bpm
        }
        decks[target]=new_track.release();
        std::cout << "[Load Complete] '" << decks[target]->get_title() << "' is now loaded on deck " << target << "\n";
        active_deck = target;
        std::cout << "[Active Deck] Switched to deck " << target << "\n";
    }
    return active_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] && track){ //checking that both tracks are valid and non-null
        int old_bpm = decks[active_deck]->get_bpm();
        int new_bpm = track->get_bpm();
        int diff = std::abs(old_bpm-new_bpm);
        bool can = diff <= bpm_tolerance;
        //std::cout << "old_bpm: "<< old_bpm <<", new_bpm: " << new_bpm <<". This is a difference of " << diff << ". The bpm_tolerance is " << bpm_tolerance << ". Is this fine? "<< can << "\n";
        return can; //checking if both bpms are compatible
    }
    return false;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] && track){ //checking that both tracks are valid and non-null
        int original_otr_bpm = track->get_bpm();
        int average_bpm = (original_otr_bpm + decks[active_deck]->get_bpm())/2; //calculating the average bpm
        track->set_bpm(average_bpm);
        std::cout << "[Sync BPM] Syncing BPM from " << original_otr_bpm << " to " << average_bpm << "\n";
    }
}
