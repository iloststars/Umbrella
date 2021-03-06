package com.agoda.umbrella.helper;


import android.util.Log;

import com.agoda.umbrella.utils.HttpUtils;

public class EydataHelper {
    /**
     * 单码登录
     * SingleCode=&Ver=&Mac=
     * 登录成功返回用户状态码,失败返回错误代码
     */
    public static String singleLogin(String api, String singleCode, String version, String mac) {
        String params = String.format("SingleCode=%s&Ver=%s&Mac=%s", singleCode, version, mac);
        String data = HttpUtils.sendPost(api, params);
        ConfigHelper.statusCode = data;
        data = data.length() == 32 ? "登录成功" : error(data);
        return data;
    }

    /**
     * 登录
     * UserName=&UserPwd=&Version=&Mac=&MacTwo=
     * 登录成功返回用户状态码,失败返回错误代码
     */
    public static String login(String api, String username, String password, String version, String mac) {
        String params = String.format("UserName=%s&UserPwd=%s&Version=%s&Mac=%s&MacTwo=%s", username, password, version, mac, mac);
        String data = HttpUtils.sendPost(api, params);
        data = data.length() == 32 ? "登录成功" : error(data);
        return data;
    }

    /**
     * 注册
     * UserName=&UserPwd=&Email=&Mac=
     * 注册成功返回1.
     */
    public static String register(String api, String username, String password, String email, String mac) {
        String params = String.format("UserName=%s&UserPwd=%s&Email=%s&Mac=%s", username, password, email, mac);
        String data = HttpUtils.sendPost(api, params);
        data = data.equals("1") ? "注册成功" : error(data);
        return data;
    }


    /**
     * 充值
     * UserName=&CardPwd=&Referral=
     * 成功返回 1,失败返回错误代码.
     */
    public static String recharge(String api, String username, String card) {
        String params = String.format("UserName=%s&CardPwd=%s", username, card);
        String data = HttpUtils.sendPost(api, params);
        data = data.equals("1") ? "充值成功" : error(data);
        return data;
    }

    /**
     * 密码找回
     * UserName=&Email=
     * 成功返回1,失败返回0.
     */
    public static String updatePassword(String api, String username, String email) {
        String params = String.format("UserName=%s&Email=%s", username, email);
        String data = HttpUtils.sendPost(api, params);
        data = data.equals("1") ? data : error(data);
        return data;
    }

    /**
     * 机器码转绑
     * UserName=&UserPwd=&Mac=&Type=
     * 成功返回1 失败返回错误代码.
     */
    public static String unbind(String api, String username, String password, String mac) {
        String params = String.format("UserName=%s&UserPwd=%s&Mac=%s", username, password, mac);
        String data = HttpUtils.sendPost(api, params);
        data = data.equals("1") ? "解绑成功" : error(data);
        return data;
    }

    public static String checkUpdate(String api, String version) {
        String params = String.format("ver=%s", version);
        String data = HttpUtils.sendPost(api, params);
        data = data.equals("1") ? "最新版本" : error(data);
        return data;
    }

    public static String getBoard(String api) {
        String data = HttpUtils.sendPost(api, " ");
        data = data.startsWith("-") ? error(data):data;
        return data;
    }

    public static String error(String ErrorCode) {
        switch (Integer.parseInt(ErrorCode)) {
            case -1:
                return "网络链接失败";
            case -2:
                return "请填写程序密钥";
            case -3:
            case -4:
                return "数据异常";
            case -5:
                return "错误的参数,请检查参数是否正确";
            case -6:
                return "还未登录";
            case -7:
                return "私人服务器,没有权限进行登录";
            case -8:
                return "账户余额不足";
            case -9:
                return "注册用户达到上限";
            case -10:
                return "VIP 插件,非 VIP 无法使用";
            case -11:
                return "开启自动状态检测失败,还未登陆!";
            case -12:
                return "开启自动状态检测失败!";
            case -13:
                return "动态算法只支持独立服务器调用";
            case -14:
                return "错误的调用";
            case -15:
                return "频繁调用,请等待10分钟后再做尝试";
            case -16:
                return "接口未开启";
            case -17:
                return "错误的调用方式,请确认后台接口的调用方式";
            case -18:
                return "服务器内部错误,请联系管理员解决";
            case -19:
                return "接口调用失败,调用次数不足";
            case -20:
                return "变量数据不存在";
            case -21:
                return "机器码一样,无需转绑";
            case -23:
                return "此接口开启了强制算法,但是没使用";
            case -101:
                return "用户名填写错误,必须以字母开头6-16位字母或数字!";
            case -102:
                return "用户不存在";
            case -103:
                return "请先登陆再调用此方法";
            case -104:
                return "密码填写错误,请输入6-16位密码";
            case -105:
                return "邮箱填写错误,请正确输入邮箱";
            case -106:
                return "用户名重复";
            case -107:
                return "邮箱重复";
            case -108:
                return "新密码输入错误";
            case -109:
                return "用户名或密码错误";
            case -110:
                return "用户使用时间已到期";
            case -111:
                return "用户未在绑定的电脑上登陆";
            case -112:
                return "用户在别的地方登陆";
            case -113:
                return "过期时间有误";
            case -114:
                return "登录数据不存在";
            case -115:
                return "用户已被禁用";
            case -116:
                return "密码修改申请过于频繁";
            case -117:
                return "未输入机器码";
            case -118:
                return "重绑次数超过限制";
            case -119:
                return "使用天数不足,重绑失败";
            case -120:
                return "注册失败,注册次数超过限制";
            case -121:
                return "用户机器码不能超过32位";
            case -122:
                return "用户已经被删除";
            case -123:
                return "用户密码输入错误";
            case -124:
                return "用户登录数达到最大";
            case -125:
                return "错误的用户操作类别";
            case -126:
                return "过期时间变更记录创建失败";
            case -127:
                return "用户充值失败";
            case -128:
                return "用户数据超过最大限制";
            case -129:
                return "用户被开发者禁止使用,请咨询开发者是否被拉到黑名单";
            case -131:
                return "用户使用次数不足";
            case -132:
                return "用户使用点数不足";
            case -133:
                return "用户状态码错误";
            case -134:
                return "用户状态码不存在";
            case -201:
                return "程序不存在";
            case -202:
                return "程序密钥输入错误";
            case -203:
                return "版本号错误";
            case -204:
                return "程序版本不存在";
            case -205:
                return "用户未申请使用程序";
            case -206:
                return "程序版本需要更新";
            case -207:
                return "程序版本已停用";
            case -208:
                return "程序未开启后台接口功能.(可在'程序'的'修改'界面开启后台接口功能";
            case -209:
                return "程序接口密码错误";
            case -210:
                return "程序停止新用户注册";
            case -211:
                return "程序不允许用户机器码转绑";
            case -301:
                return "卡密输入错误";
            case -302:
                return "卡密不存在";
            case -303:
                return "卡密已经使用";
            case -304:
                return "卡密已经过期";
            case -305:
                return "卡密已经冻结";
            case -306:
                return "卡密已经退换";
            case -307:
            case -308:
                return "卡密已经换卡";
            case -401:
                return "单码卡密错误";
            case -402:
                return "单码卡密机器码错误";
            case -403:
                return "单码卡密IP错误";
            case -404:
                return "单码卡密类型错误";
            case -405:
                return "单码卡密被禁用";
            case -406:
                return "单码卡密不存在";
            case -407:
                return "单码卡密未激活";
            case -408:
                return "单码卡密已经使用";
            case -409:
                return "单码充值卡密错误";
            case -410:
                return "单码卡密过期";
            case -420:
                return "单码卡密在别的电脑上登录";
            case -421:
                return "单码卡密超过最大登录数,如果确定已经下线,请等60分钟后重试";
            case -422:
                return "单码IP一样,无需转绑";
            case -501:
                return "单码管理信息错误";
            case -502:
                return "单码机器码转绑次数超过限制";
            case -503:
                return "单码机器码转绑后将过期";
            case -504:
                return "单码IP转绑次数超过限制";
            case -505:
                return "单码IP转绑后将过期";
            case -506:
                return "单码未开启机器码验证,无需转绑";
            case -507:
                return "单码未开启IP地址验证,无需转绑";
            case 101:
                return "充值成功!";
            case 102:
                return "充值成功!填写推荐人获赠时间失败";
            case 103:
                return "充值成功!添加推荐信息失败";
            case 104:
                return "充值成功!推荐人获赠时间失败";
            case 105:
                return "充值成功!充值的卡密类别不支持推荐人功能";
            case 106:
                return "充值成功!充值的卡密类别推荐功能已关闭";
            case 107:
                return "充值成功!成功使用推荐功能";
            case 108:
                return "充值成功!但是填写的推荐人无效";
            case 109:
                return "充值成功!已经填写过推荐人,本次无法获得推荐时间";
        }
        return "服务器连接失败";
    }
}
