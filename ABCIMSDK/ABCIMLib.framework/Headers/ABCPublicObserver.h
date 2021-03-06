//
//  ABCPublicObserver.h
//  ABCIMLib
//
//  Created by bingo on 2017/12/19.
//  Copyright © 2017年 杭州喧喧科技有限公司. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ABCStatusDefine.h"
#import "ABCMessage.h"
#import "ABCUserInfo.h"

#ifndef ABCPublicObserver_h
#define ABCPublicObserver_h

/*
 //同IM服务器连接的状态变更通知
 -(void)onConnectionStatusChanged:(ABCConnectionStatus)status {
 if (status == ConnectionStatus_Connecting) {
 self.navigationItem.title = @"连接中...";
 } else if (status == ConnectionStatus_Connected) {
 self.navigationItem.title = @"对话";
 }
 }
 */
@protocol ABCConnectionObserver <NSObject>
@optional
//同IM服务器连接的状态变更通知
-(void)onConnectionStatusChanged:(ABCConnectionStatus)status;

@end

/*!
 IMlib消息接收的监听器
 */
@protocol ABCIMClientReceiveMessageObserver <NSObject>
@optional

/*!
 接收消息的回调方法
 @param message     当前接收到的消息
 */
- (void)onReceivedMessage:(ABCMessage *)message;
    
/*!
 消息发送失败回调
 @param msgLocalID     当前消息本地id
 @param targetId       当前会话id
 */
- (void)onReceivedMessageFailure:(int)msgLocalID targetId:(NSString *)targetId;

@end

/*!
 IMlib VOIP消息接收的监听器
 */
@protocol ABCIMClientVoipMessageObserver <NSObject>
@optional

/*!
 接收消息的回调方法
 @param message     当前接收到的消息
 */
- (void)onReceivedVoipMessage:(ABCMessage *)message;
@end

/*!
 媒体消息下载的监听器
 */
@protocol ABCMediaMessageDownloadDelegate <NSObject>
@optional

/*!
 媒体消息下载成功
 */
-(void)onMediaMessageDownLoadSuccess:(ABCMessage *)message localPath:(NSString *) localPath;

/*!
 媒体消息下载进度
 */
-(void)onMediaMessageDownLoadProgress:(ABCMessage *)message progress:(float) progressValue;

/*!
 媒体消息下载失败
 */
-(void)onMediaMessageDownLoadFail:(ABCMessage *)message errorCode:(ABCErrorCode) errorCode;

@end

/*!
 媒体消息下载的监听器
 */
@protocol ABCMessageSyncStatusDelegate <NSObject>
@optional

-(void) onMessageSyncStatus:(ABCSyncStatus) syncStatus;

@end

/*!
 用户信息datasource
 */
@protocol ABCUserInfoDataSource <NSObject>

@optional

-(void) dataOfABCUserInfoByUserId:(NSString *) userId
                          success:(void (^)(ABCUserInfo *userInfo)) successBlock
                            error:(void (^)(ABCErrorCode status))errorBlock;

@end

#endif /* ABCPublicObserver_h */
