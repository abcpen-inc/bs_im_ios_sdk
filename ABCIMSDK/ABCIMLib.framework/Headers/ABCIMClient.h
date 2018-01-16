//
//  ABCIMClient.h
//  ABCIMSdk
//
//  Created by bingo on 2017/12/18.
//  Copyright © 2017年 杭州喧喧科技有限公司. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "ABCPublicObserver.h"
#import "ABCUploadMediaStatusListener.h"
#import "ABCMediaMessage.h"
#import "ABCUserInfo.h"

@interface ABCIMClient : NSObject

@property(nonatomic, strong) NSString * currUserId;

/*!
 设置当前登录用户信息，如设置此字段，在消息体传输中都会带上
 */
@property(nonatomic, strong) ABCUserInfo *currentUserInfo;


/*!
 获取笔声ABCIMClient的核心类单例
 
 @return 笔声ABCIMClient的核心类单例
 
 @discussion 您可以通过此方法，获取IMLib的单例，访问对象中的属性和方法.
 */
+ (instancetype)sharedABCIMClient;

/*!
 与笔声IM服务器建立连接
 
 @param token                   从您服务器端获取的token(用户身份令牌)
 @param userId                  当前获得用户身份令牌的用户ID
 */
- (void) connectWithToken:(NSString *) token
                   userId:(NSString *) userId;

/*!
 断开与笔声IM服务器的连接，并不再接收远程推送
 
 @discussion
 因为SDK在前后台切换或者网络出现异常都会自动重连，会保证连接的可靠性。
 所以除非您的App逻辑需要登出，否则一般不需要调用此方法进行手动断开。
 */
- (void)logout;

/*!
 设置deviceToken，用于远程推送
 
 @param deviceToken     从系统获取到的设备号deviceToken(需要去掉空格和尖括号)
 如:
 - (void)application:(UIApplication *)application
 didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
 NSString *token = [deviceToken description];
 token = [token stringByReplacingOccurrencesOfString:@"<"
 withString:@""];
 token = [token stringByReplacingOccurrencesOfString:@">"
 withString:@""];
 token = [token stringByReplacingOccurrencesOfString:@" "
 withString:@""];
 [[ABCIMClient sharedABCIMClient] bindDeviceToken:token];
 }
 */
-(void)bindDeviceToken:(NSString *)deviceToken
                success:(void (^)(void))success
                failure:(void (^)(void))fail;

/*!
 解除deviceToken绑定
 */
-(void)unBindDeviceToken:(NSString *)deviceToken
                success:(void (^)(void))success
                failure:(void (^)(void))fail;

#pragma mark - 连接状态监听

/*!
 添加连接状态Observer
 */
-(void)addConnectionObserver:(id<ABCConnectionObserver>)ob;

/*!
 移除连接状态Observer
 */
-(void)removeConnectionObserver:(id<ABCConnectionObserver>)ob;

/*!
 添加消息接收Observer
 */
-(void)addReceiveMessageObserver:(id<ABCIMClientReceiveMessageObserver>)ob;

/*!
 移除消息接收Observer
 */
-(void)removeReceiveMessageObserver:(id<ABCIMClientReceiveMessageObserver>)ob;

-(void)pushVOIPObserver:(id<ABCIMClientVoipMessageObserver>)ob;

-(void)popVOIPObserver:(id<ABCIMClientVoipMessageObserver>)ob;

#pragma mark - 程序切换到后台状态监听
-(void)enterForeground;
-(void)enterBackground;

#pragma mark - 注册自定义消息类型

/*!
 注册自定义的消息类型
 
 @param messageClass    自定义消息的类，该自定义消息需要继承于ABCMessageContent
 
 @discussion
 如果您需要自定义消息，必须调用此方法注册该自定义消息的消息类型，否则SDK将无法识别和解析该类型消息。
 */
- (void)registerMessageType:(Class)messageClass;

#pragma mark 消息发送
/*!
 发送消息
 
 @param conversationType    发送消息的会话类型
 @param targetId          发送消息的目标会话ID
 @param content             消息的内容
 @param successBlock        发送成功回调
 @param errorBlock          发送失败回调
 @return                    发送的消息实体

 */
- (ABCMessage *)sendMessage:(ABCConversationType)conversationType
                 targetId:(NSString *)targetId
                    content:(ABCMessageContent *)content
                    success:(void (^)(ABCMessage *message))successBlock
                      error:(void (^)(ABCErrorCode nErrorCode, ABCMessage *message))errorBlock;

/*!
 发送媒体消息（图片消息或文件消息）
 
 @param conversationType    发送消息的会话类型
 @param targetId          发送消息的目标会话ID
 @param content             消息的内容(必须继承与ABCMediaMessage,不然以普通Message发送)
 @param progressBlock       消息发送进度更新的回调 [progress:当前的发送进度, 0
 <= progress <= 100, message:消息实体]
 @param successBlock        发送成功回调
 @param errorBlock          发送失败回调
 */
- (ABCMessage *)sendMediaMessage:(ABCConversationType)conversationType
                      targetId:(NSString *)targetId
                         content:(ABCMessageContent *)content
                        progress:(void (^)(int progress, ABCMessage *message))progressBlock
                         success:(void (^)(ABCMessage *message))successBlock
                           error:(void (^)(ABCErrorCode nErrorCode, ABCMessage *message))errorBlock;

- (ABCMessage *)sendVOIPMessage:(ABCConversationType)conversationType
                     targetId:(NSString *)targetId
                        content:(ABCMessageContent *)content
                        success:(void (^)(ABCMessage *message))successBlock
                          error:(void (^)(ABCErrorCode nErrorCode, ABCMessage *message))errorBlock;

/*!
 插入向外发送的消息
 
 @param conversationType    会话类型
 @param targetId          目标会话ID
 @param sentStatus          发送状态
 @param content             消息的内容
 @return                    插入的消息实体
 
 */
- (ABCMessage *)insertOutgoingMessage:(ABCConversationType)conversationType
                            targetId:(NSString *)targetId
                          sentStatus:(ABCSentStatus)sentStatus
                             content:(ABCMessageContent *)content;

/*!
 插入接收的消息
 
 @param conversationType    会话类型
 @param senderUserId        发送者ID
 @param content             消息的内容
 @return                    插入的消息实体
 
 @discussion 此方法不支持聊天室的会话类型。
 */
- (ABCMessage *)insertIncomingMessage:(ABCConversationType)conversationType
                         senderUserId:(NSString *)senderUserId
                             content:(ABCMessageContent *)content;

#pragma mark - 会话列表操作
/*!
 获取会话列表
 
 @param conversationTypeList 会话类型的数组(需要将ABCConversationType转为NSNumber构建Array)
 @return                        会话ABCConversation的列表
 
 @discussion 此方法会从本地数据库中，读取会话列表。
 返回的会话列表按照时间从前往后排列，如果有置顶的会话，则置顶的会话会排列在前面。
 */
- (NSArray *)getConversationList:(NSArray *)conversationTypeList;


/*!
 获取会话中所有消息
 
 @param conversationType    会话类型
 @param targetId            目标会话ID
 @return                    消息实体ABCMessage对象列表
 */
- (NSArray *)getHistoryMessages:(ABCConversationType)conversationType
                       targetId:(NSString *)targetId;

/*!
 获取会话中，从指定消息之前、指定数量的最新消息实体

 @param conversationType    会话类型
 @param targetId            目标会话ID
 @param oldestMessageId     截止的消息ID
 @return                    消息实体ABCMessage对象列表
 */
- (NSArray *)getHistoryMessages:(ABCConversationType)conversationType
                       targetId:(NSString *)targetId
                oldestMessageId:(int) oldestMessageId
                          count:(int) count;

/*!
 消息撤回
 @param message             消息实体ABCMessage
 @param successBlock        发送成功回调
 @param errorBlock          发送失败回调
 @return                    消息实体ABCMessage
 @discussion 具体限制撤回时间点由业务层控制
 */
-(ABCMessage *) recallMessage:(ABCMessage *) message
                      success:(void (^)(ABCMessage *message))successBlock
                        error:(void (^)(ABCErrorCode nErrorCode, ABCMessage *message))errorBlock;

#pragma mark - 未读消息数
/*!
 获取某个会话内的未读消息数
 
 @param conversationType    会话类型
 @param targetId            会话目标ID
 @return                    该会话内的未读消息数
 */
- (int)getUnreadCount:(ABCConversationType)conversationType targetId:(NSString *)targetId;


/*!
 清除某个会话中的未读消息数
 
 @param conversationType    会话类型
 @param targetId            目标会话ID
 @return                    是否清除成功
 */
- (BOOL)clearMessagesUnreadStatus:(ABCConversationType)conversationType targetId:(NSString *)targetId;

/*!
获得音频或者视频本地地址

@param message      当前message
@param progress     下载进度
@param success      如果本地没有则下载并返回进度，如果已经存在直接返回地址
@param fail         下载失败
*/
- (void)getLocalUrlByMessage:(ABCMediaMessage *)message
                     progress:(void (^) (float progressValue))progress
                     success:(void (^) (NSString *localUrl))success
                     failure:(void (^)(id responseObject))fail;
#pragma mark - 用户相关

/*!
 从ABCIMLib中获取缓存的用户id
 @param userId            目标用户ID
 */
- (ABCUserInfo *)getUserInfo:(NSString *)userId;

@end