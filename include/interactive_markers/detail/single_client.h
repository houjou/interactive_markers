/*
 * single_client.h
 *
 *  Created on: Jul 17, 2012
 *      Author: gossow
 */

#ifndef SINGLE_CLIENT_H_
#define SINGLE_CLIENT_H_

#include <visualization_msgs/InteractiveMarkerInit.h>
#include <visualization_msgs/InteractiveMarkerUpdate.h>

#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <boost/function.hpp>
#include <boost/unordered_map.hpp>

#include <tf/tf.h>

#include "message_context.h"
#include "state_machine.h"
#include "../interactive_marker_client.h"


namespace interactive_markers
{

class SingleClient
{
public:

  SingleClient(
      const std::string& server_id,
      const tf::Transformer& tf,
      const std::string& target_frame,
      const InteractiveMarkerClient::CbCollection& callbacks );

  // Process message from the update channel
  void process(const visualization_msgs::InteractiveMarkerUpdate::ConstPtr& msg);

  // Process message from the init channel
  void process(const visualization_msgs::InteractiveMarkerInit::ConstPtr& msg);

  // true if INIT messages are not needed anymore
  bool isInitialized();

  // transform all messages with missing transforms
  void spin();

private:

  // check if we can go from init state to normal operation
  void checkInitFinished();

  void checkKeepAlive();

  enum StateT
  {
    INIT,
    RECEIVING,
    TF_ERROR
  };

  StateMachine<StateT> state_;

  // updateTf implementation (for one queue)
  void transformInitMsgs( );
  void transformUpdateMsgs( );

  void pushUpdates();

  void errorReset( std::string error_msg );

  // sequence number and time of last outgoing update
  uint64_t last_update_seq_num_;
  ros::Time last_update_time_;

  // true if the last outgoing update is too long ago
  // and we've already sent a notification of that
  bool update_time_ok_;

  typedef MessageContext<visualization_msgs::InteractiveMarkerUpdate> UpdateMessageContext;
  typedef MessageContext<visualization_msgs::InteractiveMarkerInit> InitMessageContext;

  // Queue of Updates waiting for tf and numbering
  typedef std::map<uint64_t, UpdateMessageContext > M_UpdateMessageContext;
  typedef std::map<uint64_t, InitMessageContext > M_InitMessageContext;

  // queue for update messages
  M_UpdateMessageContext update_queue_;

  // queue for init messages
  M_InitMessageContext init_queue_;

  const tf::Transformer& tf_;
  std::string target_frame_;

  const InteractiveMarkerClient::CbCollection& callbacks_;

  std::string server_id_;
};

}

#endif /* SINGLE_CLIENT_H_ */