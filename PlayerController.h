#ifndef PlayerControllerH
#define PlayerControllerH


#include "Input.h"
#include "FPSCamera.h"
#include "Level.h"
#include "AudioMgrFMOD.h"
#include "bullet.h"
#include "FPSCamera.h"
//handles all incoming input for the player
//kb+m aswell as controller
//is going to handle everything player related
using namespace DirectX::SimpleMath;

class PlayerController {
public:
	PlayerController();
	~PlayerController();

	vector<Bullet*>pBullets;
	Vector3 pb_pos;   //player bullet position
	Vector3 pb_dir;   //player bullet direction
	float pb_speed;   // player bullet speed

	void Update(float dTime);
	void Render(float dTime);

	bool isAlive() { return alive; }
	void initialise();
	Level* ptrLevel;
	//FPSCamera* ptrCam;
	//Model pModel; //will need to get bullet spawning from gun eventually
	bool flashlightDown;
	bool interactDown;
	int maxAmmo = 30;
	int ammo = 30;
	int magMax = 6;
	int mag = 3;
	float bSpeed;
	int health = 100;
	int healthMax = 100;
	int score = 0;
	void takeDamage(int);
	Vector3 GetPos();

	bool GetMouseLastInput();
	int coordX, coordZ;
	float GetControllerX();
	float GetControllerY();
private:
	unsigned int sfxHdlgun = 1;
	unsigned int sfxHdlrun = 1;
	unsigned int sfxHdlwalk = 1;
	unsigned int sfxHdlspaceBG = 1;
	float sfxvolume = 1;
	float sense = 0.75;
	
	void handleInputs(float);
	void flashlight();
	void ammoReload();
	
	//void fireBullet();
	bool rTriggerHeldDown = false;
	bool lTriggerHeldDown = false;
	bool reloadHeldDown = false;
	bool flashlightHeldDown = false;
	bool flashlightOn = false;
	bool alive = true;
	bool playerMoved = false;
	void fireBullet(Vector3 bulletDirection, Vector3 position, float speed);
	FPSCamera ptrCam;
	Gamepad mPad;
	
	Model mGun;

	// Some Extra Gun Stuff
	bool ads = false;
	Vector3 adsPos = Vector3(0.3, -0.2, 1);
	Vector3 standardPos = Vector3(0, -0.2, 1);
	Vector3 cGunPos = standardPos;
	bool muzzleFlash = false;
	void muzzleFlashActivate(float dTime);
	void muzzleFlashDeactivate();
	float flashTime = 0.5;
	float currentFlashTime = 0;

	// gun type
	typedef enum { SINGLE, BURST, AUTO } Firemode;
	Firemode fireMode;

	// gun stats
	bool empty = false;

	float msBetweenShots = 100;
	float muzzleVelocity = 35;
	float tTime = 0;
	int burstCount = 3;
	float nextShotTime = 0;
	bool triggerReleasedSinceLastShot = true;
	int shotsRemainingInBurst = burstCount;

	void Shoot(float dTime, Vector3 camDir, Vector3 lookDir);

	bool m_MouseLastInput;

	// Weapon Sway
	float m_swayAmount		= 0.2;
	float m_maxSwayAmount	= 0.3;
	float m_swaySmooth		= 0.1;
	float m_rTOffsetFix		= 2.66; // fixes the rotation issue
	float m_factorX = 0, m_factorY = 0, m_factorZ = 0;
	int   m_mouseIntputType = 0;
	int   m_mouseSwayFix = 20; // Used to remove 'twitchiness' of sway animation

	// Weapon Bob
	float m_bobTimer	= 0.0;
	float m_bobSpd		= 0.03;
	float m_bobAmt		= 0.02;
	float m_bobOffset	= 0.2;
	float m_bobMidPnt	= 2.0;
	float m_waveSlice;
	float m_hor;
	float m_vert;
	float m_totVerts;
	float m_totAxes;
	float m_translateChange;
	void  HandleWeaponBob(float dTime);
	float clamp(float n, float lower, float upper);
	int	  m_inputType = 0;
	bool  m_pressed = false;
	// Weapon Recoil
	Vector2 m_kickMinMax			= Vector2(.05f, .2f);
	Vector2 m_recoilAngleMinMax		= Vector2(3.0f, 5.0f);
	float m_recoilMoveSettleTime	= .1f;
	float m_recoilRotSettleTime		= .1f;
	Vector3 m_recoilSmoothDampVel = Vector3(0,0,1);
	float m_recoilRotSmoothDampVel= 1;
	float m_recoilAngle;
	void RecoilHandle(float dTime);
	float lerp(float a, float b, float f);
};
#endif // !PlayerControllerH