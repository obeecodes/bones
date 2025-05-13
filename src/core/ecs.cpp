#include "../../include/core/ecs.h"
#include "../../include/components/audio.h"
#include "../../include/components/collider.h"
#include "../../include/components/health.h"
#include "../../include/components/input.h"
#include "../../include/components/physics.h"
#include "../../include/components/script.h"
#include "../../include/components/sprite.h"
#include "../../include/components/transform2d.h"
#include "../../include/components/ui.h"
#include <iostream>
#include "assert.h"
#include <algorithm>


template <typename T>
ECS<T>::ECS(){
    inactiveIndex = 0;
    
    components = std::vector<T>(25600);
    denseComponents = std::vector<uint32_t>(25600);
    sparseComponents = std::vector<uint32_t>(25600);
    
}

// if I need to, I could keep track of inactive components in another data structure
/*Resize logic fails if new memory location is allocated*/
template <typename T>
void ECS<T>::createComponent(T component, uint32_t entityID){
    //you could also add code to overwrite an old component
    
    if(inactiveIndex >= components.size()){
        components.resize(components.size() * 2);
    }
    components[inactiveIndex] = component; //adds new component to vector
    denseComponents[inactiveIndex] = entityID; //adds new entity to denseComponents



    //if entityID > sparseComponents.size() -> seg fault
    //check that sparseComponents.size() > entityID before insertion
    if(sparseComponents.size() < entityID){
        sparseComponents.resize(std::max(sparseComponents.size() * 2, static_cast<size_t>(entityID + 1)));
        sparseComponents[entityID] = inactiveIndex; // sets index of entity in sparseComponents
    }else{
        sparseComponents[entityID] = inactiveIndex; // sets index of entity in sparseComponents
    }

    //adds entity to set for quick lookup
    active.insert(entityID);
    inactiveIndex++;

    // resize dense
    if(inactiveIndex == denseComponents.size() - 1){
        denseComponents.resize(denseComponents.size() * 2);
    }


}

template <typename T>
void ECS<T>::destroyComponent(uint32_t entityID){

    // If entity does not have an associated component, return
    if (this->active.find(entityID) == this->active.end()) {
        return;
    }

    uint32_t iA = denseComponents[sparseComponents[entityID]]; 
    uint32_t iB = denseComponents[inactiveIndex - 1]; 

    uint32_t pA = sparseComponents[entityID];

    // swap entity component to last position in vector
    std::swap(components[sparseComponents[entityID]], components[inactiveIndex - 1]);
    std::swap(denseComponents[sparseComponents[entityID]], denseComponents[inactiveIndex - 1]);
    sparseComponents[entityID] = iA;
    sparseComponents[iB] = pA;
    
    // sets inactive status to entity
    this->active.erase(entityID);

    inactiveIndex--;

}

template <typename T>
std::vector<T>& ECS<T>::getComponents(){
    return components;
}

template <typename T>
uint32_t& ECS<T>::getInactiveIndex(){
    return inactiveIndex;
}

template <typename T>
T ECS<T>::getEntityComponent(uint32_t entityID){
    return components[sparseComponents[entityID]];
}

template <typename T>
uint32_t ECS<T>::getEntityIndex(uint32_t entityID){

    if(entityID != 0 && denseComponents[sparseComponents[entityID]] == 0 ){
        return UINT32_MAX;
    }

    return sparseComponents[entityID];
}

// given that I created a vector to ensure that this invariant is true: sparse[ dense[ i ] ] = i 
// I can use this to get the corresponding entityID from any component in a system update loop
template <typename T>
uint32_t ECS<T>::getEntityID(uint32_t index){
    if(index != 0 && sparseComponents[denseComponents[index]] == 0 ){
        return UINT32_MAX;
    }

    assert(index == sparseComponents[denseComponents[index]]);
    return denseComponents[index];
}

template <typename T>
bool ECS<T>::hasComponent(uint32_t entityID){
    return active.find(entityID) != active.end();
}

template class ECS<Audio>;
template class ECS<Collider>;
template class ECS<Health>;
template class ECS<Input>;
template class ECS<Physics>;
template class ECS<Script>;
template class ECS<Sprite>;
template class ECS<Transform2D>;
template class ECS<UI>;
    
