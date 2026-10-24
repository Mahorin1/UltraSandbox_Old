//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
#pragma once
#include <audio.hpp>
#include <physic.hpp>
#include <sw/Line.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
enum class EntityName {
  Saw,
  Barrel,
  PhysicRect,
};
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
class SpawnEntity {
  //-------------//
public:
  SpawnEntity(Collider *c, sf::Vector2f origin);
  virtual ~SpawnEntity() = default;
  //-------------//
  SpawnEntity(const SpawnEntity &) = delete;
  SpawnEntity &operator=(const SpawnEntity &) = delete;
  //-------------//
  virtual void update(float dt);
  virtual void draw(sf::RenderWindow &window) = 0;
  //-------------//
  sf::Vector2f getPos();
  float getAngle();
  //-------------//
  void setSoundBuffer(sf::SoundBuffer *sb);
  void setEvery(Every *e);
  void setSoundSpeed(float s);
  void supr(bool anim = false);
  bool is_supr(bool anim = false);
  void destroy();
  //-------------//
  Collider *getCorps();
  //-------------//
protected:
  float m_angle;
  Every *m_every;
  sf::Color m_color;
  float m_soundSpeed;
  sf::Vector2f m_pos;
  sf::Clock m_soundClock;
  bool m_supr, m_animSupr;
  sf::SoundBuffer *m_bufferPtr;
  sf::Transformable m_transform;
  Tweener<float, float> m_tween;
  Collider *m_corps;
  float m_angleOffset;
  //-------------//
};
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
class PhysicRect final : public SpawnEntity {
public:
  //-------------//
  PhysicRect(sf::Vector2f pos, sf::Vector2f dims);
  //-------------//
  void draw(sf::RenderWindow &w) override;
  //-------------//
private:
  //-------------//
  sf::RenderTexture m_texture;
  sf::Sprite m_sprite;
  //-------------//
};
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
class Barrel final : public SpawnEntity {
public:
  //-------------//
  Barrel(sf::Vector2f pos, sf::Texture *t, float size = 0.8f);
  //-------------//
  void draw(sf::RenderWindow &w) override;
  //-------------//
private:
  //-------------//
  sf::RectangleShape m_shape;
  //-------------//
};
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
class Saw final : public SpawnEntity {
public:
  //-------------//
  Saw(sf::Vector2f pos, sf::Texture *t, float size = 0.8f);
  //-------------//
  void draw(sf::RenderWindow &w) override;
  //-------------//
private:
  //-------------//
  sf::Sprite m_sprite;
  //-------------//
};
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
class Robot {
  //-------------//
public:
  Robot(sf::Vector2f pos);
  void update(float dt);
  void draw(sf::RenderWindow &w);
  //-------------//
  ShapedCollider *m_chest;
  ShapedCollider *m_wheel;
  //-------------//
private:
  b2WheelJoint *m_joint;
  sf::CircleShape m_circ;
  PIDcontroller m_balancer;
  PIDcontroller m_posKeeper;
  bool m_posKeepMode;
  float m_posKeep;
  float m_speed;
  float m_accel;
  float m_maxSpeed;
  bool m_keySwitch;
  bool m_keySwitch2;
  bool m_jumpSwitch;
  bool m_onGround;
  int m_nbContact;
  //-------------//
};
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
class Guirlande {
  //-------------//
public:
  Guirlande();
  void update(sf::Vector2f pos);
  void draw(sf::RenderWindow &w);
  //-------------//
private:
  sf::CircleShape m_circle;
  sw::Line m_line;
  b2Body *m_bodyA;
  b2Body *m_bodyB;
  b2MouseJoint *m_anchorJ;
  b2DistanceJoint *m_linkJ;
  //-------------//
};
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//