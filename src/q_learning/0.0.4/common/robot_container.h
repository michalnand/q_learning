#ifndef _ROBOT_CONTAINER_H_
#define _ROBOT_CONTAINER_H_
 
#include "common.h"

/*

struct sRobotPacket
{
	u32 robot_id;
	u32 type;
	u32 part;
	u32 crc;
	u32 size;

	u8 data[ROBOT_PACKET_DATA_SIZE];
};
*/

class CRobotContainer
{
	private:
		std::mutex	vector_mutex;

		std::vector<std::mutex*> mutex;

		std::vector<struct sRobotPacket*> robots;

	public:
		CRobotContainer();
		~CRobotContainer();

		void update(struct sRobotPacket robot_packet);
		struct sRobotPacket get(u32 idx);
		u32 get_count();
};


#endif