#include "../../include/systems/quadtree.h"

Quadtree::Quadtree(Rectangle region)
    : region(region){
    this->level = 1;
}

Quadtree::Quadtree(uint32_t level, Rectangle region)
    : level(level), region(region){
    for(uint32_t i = 0; i < 4; i++){
        nodes[i] = nullptr;
    }
}

void Quadtree::clear(){
    regionComponents.clear();

    for(uint32_t i = 0; i < 4; i++){
        if(nodes[i] != nullptr){
            nodes[i]->clear();
            delete nodes[i];
            nodes[i] = nullptr;
        }
    }
}

void Quadtree::insert(Collider collider){
    if(nodes[0] != nullptr){
        uint32_t index = getIndex(collider.rect);

        if(index != UINT32_MAX){
            nodes[index]->insert(collider);
            return;
        }
    }

    regionComponents.push_back(collider);

    if(regionComponents.size() > MAX_OBJECTS && level < MAX_DEPTH){
        if(nodes[0] == nullptr){
            split();
        }

        uint32_t i = 0;
        while(i < regionComponents.size()){
            uint32_t index = getIndex(regionComponents[i].rect);
            if(index != UINT32_MAX){
                nodes[index]->insert(regionComponents[i]);
                regionComponents.erase(regionComponents.begin() + i);
            } else {
                i++;
            }
        }
    }
}

void Quadtree::remove(Collider collider){
    uint32_t index = getIndex(collider.rect);
    if(index != UINT32_MAX && nodes[0] != nullptr){
        nodes[index]->remove(collider);
    } else {
        //auto it = std::find(regionComponents.begin(), regionComponents.end(), collider);
        //if(it != regionComponents.end()){
          //  regionComponents.erase(it);
     //   }
    }
}

void Quadtree::retrieve(std::vector<Collider>& nearComponents, const Rectangle& rect) const{
    uint32_t index = getIndex(rect);
    if(index != UINT32_MAX && nodes[0] != nullptr){
        nodes[index]->retrieve(nearComponents, rect);
    }

    nearComponents.insert(nearComponents.end(), regionComponents.begin(), regionComponents.end());
}

void Quadtree::split(){
    float subWidth = region.width / 2;
    float subHeight = region.height / 2;
    float x = region.x;
    float y = region.y;

    nodes[0] = new Quadtree(level + 1, Rectangle{ x + subWidth, y, subWidth, subHeight});
    nodes[1] = new Quadtree(level + 1, Rectangle{ x, y, subWidth, subHeight});
    nodes[2] = new Quadtree(level + 1, Rectangle{ x, y + subHeight, subWidth, subHeight});
    nodes[3] = new Quadtree(level + 1, Rectangle{ x + subWidth, y + subHeight, subWidth, subHeight});
}


uint32_t Quadtree::getIndex(const Rectangle& rect) const {
    uint32_t index = UINT32_MAX;
    int verticalMidpoint = region.x + (region.width / 2);
    int horizontalMidpoint = region.y + (region.height / 2);

    bool topQuads = (rect.y < horizontalMidpoint && rect.y + rect.height < horizontalMidpoint);
    bool bottomQuads = (rect.y > horizontalMidpoint);

    if(rect.x < verticalMidpoint && rect.x + rect.width < verticalMidpoint){
        if(topQuads){
            index = 1;
        } else if (bottomQuads){
            index = 2;
        }
    } else if(rect.x > verticalMidpoint){
        if(topQuads){
            index = 0;
        }else if(bottomQuads){
            index = 3;
        }
    }
    return index;
}