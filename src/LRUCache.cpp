#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string& track_id) const {
    return findSlot(track_id) != max_size;
}

AudioTrack* LRUCache::get(const std::string& track_id) {
    size_t idx = findSlot(track_id);
    if (idx == max_size) return nullptr;
    return slots[idx].access(++access_counter);
}

/**
 * TODO: Implement the put() method for LRUCache
 */
bool LRUCache::put(PointerWrapper<AudioTrack> track) {
    if(!track){ //chekcing that we didn't get a null track
        return false;
    }
    //checking if there is a track with the same title in the cache
    for(size_t i=0;i<max_size;i++){
        if(slots[i].isOccupied() && slots[i].getTrack()->get_title()==track->get_title()){
            slots[i].access(++access_counter); // updating the track's access time
            return false;
        }
    }
    bool evicted = false;
    if(size()==max_size){ //checking if the cache is full, then emptying a slot if needed
        evictLRU();
        evicted = true;
    }
    size_t empty_slot = findEmptySlot();
    slots[empty_slot].store(std::move(track),++access_counter);
    //slots[empty_slot].store(PointerWrapper<AudioTrack>(track.release()),access_counter);
    return evicted;
}

bool LRUCache::evictLRU() {
    size_t lru = findLRUSlot(); //finding the least recently used slot
    if (lru == max_size || !slots[lru].isOccupied()) return false; //lru==max_size means that the cache is empty, the second one means that there is a free slot, and in both cases there is no need to evict a slot
    slots[lru].clear(); //if the cache is really full, clear the least recently used slot
    return true;
}

size_t LRUCache::size() const {
    size_t count = 0;
    for (const auto& slot : slots) if (slot.isOccupied()) ++count;
    return count;
}

void LRUCache::clear() {
    for (auto& slot : slots) {
        slot.clear();
    }
}

void LRUCache::displayStatus() const {
    std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
    for (size_t i = 0; i < max_size; ++i) {
        if(slots[i].isOccupied()){
            std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                      << " (last access: " << slots[i].getLastAccessTime() << ")\n";
        } else {
            std::cout << "  Slot " << i << ": [EMPTY]\n";
        }
    }
}

size_t LRUCache::findSlot(const std::string& track_id) const {
    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id) return i;
    }
    return max_size;

}

/**
 * TODO: Implement the findLRUSlot() method for LRUCache
 */
size_t LRUCache::findLRUSlot() const {
    bool found = false;
    size_t min_index=0;
    for(size_t i=0;i<max_size;i++){
        if(slots[i].isOccupied())
        {
            if(!found){
                min_index = i;
                found = true;
            }
            else{
                if(slots[i].getLastAccessTime()<slots[min_index].getLastAccessTime()){
                    min_index = i;
                }
            }
        }
    }
    if(found){
        return min_index;
    }
    return max_size;
}

size_t LRUCache::findEmptySlot() const {
    for (size_t i = 0; i < max_size; ++i) {
        if (!slots[i].isOccupied()) return i;
    }
    return max_size;
}

void LRUCache::set_capacity(size_t capacity){
    if (max_size == capacity)
        return;
    //udpate max size
    max_size = capacity;
    //update the slots vector
    slots.resize(capacity);
}