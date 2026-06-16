#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <vector>
#include <limits>

const int WIDTH = 800, HEIGHT = 600;

struct Vec3 {
    float x, y, z;
    Vec3(float x=0,float y=0,float z=0):x(x),y(y),z(z){}
    Vec3 operator+(const Vec3& b)const{return{x+b.x,y+b.y,z+b.z};}
    Vec3 operator-(const Vec3& b)const{return{x-b.x,y-b.y,z-b.z};}
    Vec3 operator*(float t)const{return{x*t,y*t,z*t};}
    Vec3 operator*(const Vec3& b)const{return{x*b.x,y*b.y,z*b.z};}
    float dot(const Vec3& b)const{return x*b.x+y*b.y+z*b.z;}
    float len()const{return std::sqrt(x*x+y*y+z*z);}
    Vec3 norm()const{float l=len();return{x/l,y/l,z/l};}
    Vec3 reflect(const Vec3& n)const{return *this - n*(2*dot(n));}
};

struct Ray { Vec3 o, d; };

struct Sphere {
    Vec3 center; float radius;
    Vec3 color; float reflectivity;
    bool hit(const Ray& r, float& t) const {
        Vec3 oc = r.o - center;
        float a=r.d.dot(r.d), b=2*oc.dot(r.d), c=oc.dot(oc)-radius*radius;
        float disc=b*b-4*a*c;
        if(disc<0) return false;
        float t1=(-b-std::sqrt(disc))/(2*a);
        if(t1>0.001f){t=t1;return true;}
        float t2=(-b+std::sqrt(disc))/(2*a);
        if(t2>0.001f){t=t2;return true;}
        return false;
    }
};

struct Plane {
    Vec3 point, normal, color;
    float reflectivity;
    bool hit(const Ray& r, float& t) const {
        float denom=normal.dot(r.d);
        if(std::abs(denom)<1e-6f) return false;
        t=normal.dot(point-r.o)/denom;
        return t>0.001f;
    }
};

struct Light { Vec3 pos, color; };

std::vector<Sphere> spheres;
std::vector<Light> lights;
Plane floor_plane;

Vec3 traceRay(const Ray& ray, int depth=0);

bool shadowCheck(const Vec3& point, const Vec3& lightPos) {
    Vec3 dir=(lightPos-point).norm();
    Ray sr{point,dir};
    float maxT=(lightPos-point).len();
    for(auto& s:spheres){float t;if(s.hit(sr,t)&&t<maxT)return true;}
    float t;
    if(floor_plane.hit(sr,t)&&t<maxT)return true;
    return false;
}

Vec3 shade(const Vec3& point, const Vec3& normal, const Vec3& objColor, float refl, const Ray& ray, int depth) {
    Vec3 color{0,0,0};
    Vec3 ambient = objColor * 0.1f;
    color = ambient;
    for(auto& l:lights){
        if(shadowCheck(point,l.pos)) continue;
        Vec3 ld=(l.pos-point).norm();
        float diff=std::max(0.f,normal.dot(ld));
        Vec3 diffuse=objColor*l.color*diff;
        Vec3 ref=ld.reflect(normal).norm();
        Vec3 view=(ray.o-point).norm();
        float spec=std::pow(std::max(0.f,ref.dot(view)),32);
        Vec3 specular=l.color*spec*0.5f;
        color=color+diffuse+specular;
    }
    if(refl>0&&depth<3){
        Vec3 refDir=ray.d.reflect(normal).norm();
        Ray refRay{point,refDir};
        Vec3 refColor=traceRay(refRay,depth+1);
        color=color*(1-refl)+refColor*refl;
    }
    return {std::min(1.f,color.x),std::min(1.f,color.y),std::min(1.f,color.z)};
}

Vec3 traceRay(const Ray& ray, int depth) {
    float minT=1e9; int hitObj=-1; bool hitFloor=false;
    for(int i=0;i<(int)spheres.size();i++){
        float t;
        if(spheres[i].hit(ray,t)&&t<minT){minT=t;hitObj=i;hitFloor=false;}
    }
    float ft;
    if(floor_plane.hit(ray,ft)&&ft<minT){minT=ft;hitFloor=true;hitObj=-1;}
    if(hitFloor){
        Vec3 p=ray.o+ray.d*minT;
        return shade(p,floor_plane.normal,floor_plane.color,floor_plane.reflectivity,ray,depth);
    }
    if(hitObj>=0){
        Vec3 p=ray.o+ray.d*minT;
        Vec3 n=(p-spheres[hitObj].center).norm();
        return shade(p,n,spheres[hitObj].color,spheres[hitObj].reflectivity,ray,depth);
    }
    float t2=0.5f*(ray.d.norm().y+1.f);
    return Vec3{1,1,1}*(1-t2)+Vec3{0.5f,0.7f,1.f}*t2;
}

void setupScene(float time) {
    spheres.clear();
    spheres.push_back({{-1.5f,0.f,-5.f},1.f,{1.f,0.2f,0.2f},0.1f});
    spheres.push_back({{1.5f,0.f,-5.f},1.f,{0.2f,0.2f,1.f},0.1f});
    float mx=std::sin(time)*0.5f;
    spheres.push_back({{mx,1.f+std::cos(time)*0.3f,-4.f},0.6f,{0.8f,0.8f,0.8f},0.8f});
    floor_plane={{0,-1,0},{0,1,0},{0.4f,0.4f,0.4f},0.3f};
    lights.clear();
    lights.push_back({{-3,5,-3},{1,0.8f,0.8f}});
    lights.push_back({{3,4,-2},{0.8f,0.8f,1.f}});
}

int main() {
    sf::RenderWindow window(sf::VideoMode({(unsigned)WIDTH,(unsigned)HEIGHT}), "Ray Tracer");
    sf::Image img({(unsigned)WIDTH,(unsigned)HEIGHT});
    sf::Texture tex;
    tex.resize({(unsigned)WIDTH,(unsigned)HEIGHT});
    sf::Sprite sprite(tex);
    sf::Clock clock;
    bool needRender=true;
    float animTime=0;

    while(window.isOpen()){
        while(auto ev=window.pollEvent()){
            if(ev->is<sf::Event::Closed>()) window.close();
            if(auto* k=ev->getIf<sf::Event::KeyPressed>()){
                if(k->code==sf::Keyboard::Key::Escape) window.close();
                if(k->code==sf::Keyboard::Key::R) needRender=true;
            }
        }
        animTime+=clock.restart().asSeconds();
        needRender=true;
        if(needRender){
            setupScene(animTime);
            Vec3 cam{0,0.5f,1};
            float fov=1.f;
            for(int y=0;y<HEIGHT;y++){
                for(int x=0;x<WIDTH;x++){
                    float u=(x+0.5f)/WIDTH*2-1;
                    float v=1-(y+0.5f)/HEIGHT*2;
                    Vec3 dir{u*fov*(float)WIDTH/HEIGHT,v*fov,-1};
                    Ray ray{cam,dir.norm()};
                    Vec3 c=traceRay(ray);
                    img.setPixel({(unsigned)x,(unsigned)y},
                        sf::Color((uint8_t)(c.x*255),(uint8_t)(c.y*255),(uint8_t)(c.z*255)));
                }
            }
            tex.update(img);
            needRender=false;
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}
