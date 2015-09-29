#include "robot_container.h" 


CRobotContainer::CRobotContainer()
{

}

CRobotContainer::~CRobotContainer()
{

}

void CRobotContainer::update(struct sRobotPacket robot_packet)
{

	u32 i;

	i32 idx = -1;

	u32 size = robots.size();

	for (i = 0; i < size; i++)
	{
		mutex[i]->lock();
		if (robots[i]->robot_id == robot_packet.robot_id)
			idx = i;
		mutex[i]->unlock();
	}

	if (idx == -1)
	{
		vector_mutex.lock();

		mutex.push_back(new std::mutex());

		struct sRobotPacket *robot_packet_tmp;

		robot_packet_tmp = new struct sRobotPacket;
		*robot_packet_tmp = robot_packet;

		mutex[mutex.size()-1]->lock();
		robots.push_back(robot_packet_tmp);
		mutex[mutex.size()-1]->unlock();

		vector_mutex.unlock();		
	}
	else
	{
		mutex[idx]->lock();
		*robots[idx] = robot_packet;
		mutex[idx]->unlock();	
	}
}

struct sRobotPacket CRobotContainer::get(u32 idx)
{
	struct sRobotPacket res;

	res.robot_id = 0;

	if (idx < robots.size())
	{
		mutex[idx]->lock();
		res = *robots[idx];
		mutex[idx]->unlock();
	}

	return res;
}


u32 CRobotContainer::get_count()
{
	u32 size = robots.size();

	return size;
}