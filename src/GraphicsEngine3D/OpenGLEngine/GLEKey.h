#ifndef GLE_KEY_H
#define GLE_KEY_H

class GLEKey {
public:
	GLEKey();
	GLEKey(const GLEKey &other);
	GLEKey(const unsigned char &id);
	~GLEKey() = default;

	void down();
	void up();

	bool isPressed();
	bool isHeld();
	
	unsigned char getId() const;
	void setId(const unsigned char &id);
private:
	unsigned char _id;
	bool _held;
	bool _registered;
};

#endif // GLE_KEY_H
