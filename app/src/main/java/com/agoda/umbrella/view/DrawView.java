
package com.agoda.umbrella.view;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.RectF;
import android.graphics.SurfaceTexture;
import android.graphics.Typeface;
import android.util.Log;
import android.view.TextureView;

import androidx.annotation.NonNull;

import com.agoda.umbrella.data.DataParser;
import com.agoda.umbrella.data.DataProvider;
import com.agoda.umbrella.helper.ConfigHelper;
import com.agoda.umbrella.helper.DrawHelper;
import com.agoda.umbrella.helper.IconHelper;
import com.agoda.umbrella.utils.ScreenUtils;

import static com.agoda.umbrella.data.DataParser.buffArray;
import static com.agoda.umbrella.data.DataParser.heroArray;


public class DrawView extends TextureView implements TextureView.SurfaceTextureListener, Runnable {
    private Canvas canvas;
    private Thread drawThread;
    private boolean threadFlag;
    public static boolean controlFlag = true;


    public DrawView(Context context) {
        super(context);
        init();
    }

    private void init() {
        setSurfaceTextureListener(this);
        drawThread = new Thread(this);
    }

    @Override
    public void onSurfaceTextureAvailable(@NonNull SurfaceTexture surface, int width, int height) {
        threadFlag = true;
        drawThread.start();//开启绘制线程
    }


    @Override
    public void onSurfaceTextureSizeChanged(@NonNull SurfaceTexture surface, int width, int height) {

    }

    @Override
    public boolean onSurfaceTextureDestroyed(@NonNull SurfaceTexture surface) {
        //Log.e("mylog", "onSurfaceTextureDestroyed: ");
        threadFlag = false;
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(@NonNull SurfaceTexture surface) {

    }

    @Override
    public void run() {
        //Log.d("mylog", "run: ");
        float icon_x, icon_y;
        float ray_x, ray_y;

        float[][] monster_location0 = {
                {-23600, 690}, // 蓝方 蓝区 蓝Buff 0
                {-30170, -9820}, // 蓝方 蓝区 狼
                {-36190, 4500}, // 蓝方 蓝区 蜥蜴
                {2500, -30000}, // 蓝方 红区 红Buff 3
                {-3700, -19200}, // 蓝方 红区 猪
                {16250, -36790}, // 蓝方 红区 鸟
                {23600, -690}, // 红方 蓝区 蓝Buff 6
                {30270, 9710}, // 红方 蓝区 狼
                {36300, -4280}, // 红方 蓝区 蜥蜴
                {-2500, 30000}, // 红方 红区 红Buff 9
                {3450, 19260}, // 红方 红区 猪
                {-15950, 36170}, // 红方 红区 鸟
        };

        float[][] monster_location = {
                {0, 0}, // 蓝方 蓝区 蓝Buff
                {0, 0}, // 蓝方 蓝区 狼
                {0, 0}, // 蓝方 蓝区 蜥蜴
                {0, 0}, // 蓝方 红区 红Buff
                {0, 0}, // 蓝方 红区 猪
                {0, 0}, // 蓝方 红区 鸟
                {0, 0}, // 红方 蓝区 蓝Buff
                {0, 0}, // 红方 蓝区 狼
                {0, 0}, // 红方 蓝区 蜥蜴
                {0, 0}, // 红方 红区 红Buff
                {0, 0}, // 红方 红区 猪
                {0, 0}, // 红方 红区 鸟
        };

        float k = IconHelper.k;
        Paint paint = new Paint();
        Paint paint_start = new Paint();
        Paint paint_red = new Paint();
        Paint paint_gray = new Paint();
        Paint paint_edge = new Paint();
        Paint paint_buffText = new Paint();
        Paint paint_animalText = new Paint();
        Paint paint_buffRed = new Paint();
        Paint paint_buffBlue = new Paint();
        Paint paint_buffYellow = new Paint();
        Paint paint_backhome = new Paint();
        Paint paint_blood = new Paint();
        Paint paint_ray = new Paint();
        Paint paint_rect = new Paint();
        Paint paint_ready = new Paint();
        Paint paint_hint = new Paint();
        Paint paint_cd = new Paint();
        Paint paint_cd_text = new Paint();
        Paint paint_cd_big = new Paint();
        Paint paint_deadline = new Paint();
        Paint paint_name = new Paint();
        Paint paint_bedge = new Paint();
        Paint paint_soldier = new Paint();


        //等待对局开始文字
        paint_start.setColor(Color.WHITE);
        paint_start.setStyle(Paint.Style.FILL);
        paint_start.setTextSize(30f * IconHelper.k);
        paint_start.setTextAlign(Paint.Align.CENTER);

        //红色画笔--圆圈
        paint_red.setColor(Color.RED);
        paint_red.setStyle(Paint.Style.STROKE);
        paint_red.setAntiAlias(true);
        paint_red.setDither(true);
        paint_red.setStrokeWidth(3f * IconHelper.k);


        //灰色画笔--血条框
        paint_gray.setColor(Color.GRAY);
        paint_gray.setStyle(Paint.Style.STROKE);
        paint_gray.setAntiAlias(true);
        paint_gray.setDither(true);
        paint_gray.setStrokeWidth(6f * IconHelper.k);

        //血条边缘画笔
        paint_bedge.setColor(Color.GRAY);
        paint_bedge.setAntiAlias(true);
        paint_bedge.setDither(true);
        paint_bedge.setStyle(Paint.Style.FILL);
        paint_bedge.setStrokeWidth(1f * IconHelper.k);

        //边缘画笔
        paint_edge.setColor(Color.WHITE);
        paint_edge.setAntiAlias(true);
        paint_edge.setDither(true);
        paint_edge.setStyle(Paint.Style.FILL);
        paint_edge.setStrokeWidth(2f * IconHelper.k);

        //red buff ready
        paint_buffRed.setColor(Color.RED);
        paint_buffRed.setStyle(Paint.Style.FILL);
        paint_buffRed.setStrokeWidth(5f * IconHelper.k);

        //yellow buff ready
        paint_buffYellow.setColor(Color.YELLOW);
        paint_buffYellow.setStyle(Paint.Style.FILL);
        paint_buffYellow.setStrokeWidth(5f * IconHelper.k);

        //blue buff ready
        paint_buffBlue.setColor(Color.parseColor("#5677FC"));
        paint_buffBlue.setStyle(Paint.Style.FILL);
        paint_buffBlue.setStrokeWidth(5f * IconHelper.k);

        //red and blue buff text
        paint_buffText.setColor(Color.WHITE);
        paint_buffText.setStyle(Paint.Style.FILL);
        paint_buffText.setFakeBoldText(true);
        paint_buffText.setTextSize(25f * IconHelper.k);
        paint_buffText.setTextAlign(Paint.Align.CENTER);

        //血条画笔
        paint_blood.setColor(Color.rgb(223, 57, 49));
        paint_blood.setStyle(Paint.Style.STROKE);
        paint_blood.setAntiAlias(true);
        paint_blood.setDither(true);
        paint_blood.setStrokeWidth(6f * IconHelper.k);

        //射线画笔
        paint_ray.setColor(Color.WHITE);
        paint_ray.setStyle(Paint.Style.STROKE);
        paint_ray.setStrokeWidth(1.4f * IconHelper.k);
        paint_ray.setAlpha(220);
        paint_ray.setAntiAlias(true);       //设置抗锯齿
        paint_ray.setDither(true);

        //方框画笔
        paint_rect.setColor(Color.parseColor("#00FFFF"));
//        paint_rect.setColor(Color.WHITE);
        paint_rect.setStyle(Paint.Style.STROKE);
        paint_rect.setStrokeWidth(4f * IconHelper.k);
        paint_rect.setAntiAlias(true);       //设置抗锯齿
        paint_rect.setDither(true);

        paint_backhome.setColor(Color.parseColor("#00F5FF"));
        paint_backhome.setStyle(Paint.Style.FILL);

        //技能就绪画笔
        paint_ready.setColor(Color.YELLOW);
        paint_ready.setStyle(Paint.Style.FILL);
        paint_ready.setStrokeWidth(8f * IconHelper.k);

        //技能提示画笔
        paint_hint.setColor(Color.parseColor("#00FFFF"));
        paint_hint.setStyle(Paint.Style.FILL);
        paint_hint.setTextSize(30f * IconHelper.k);
        paint_hint.setAntiAlias(true);       //设置抗锯齿
        paint_hint.setDither(true);
        paint_hint.setTextAlign(Paint.Align.CENTER);

        //技能CD画笔
        paint_cd.setColor(Color.RED);
        paint_cd.setStyle(Paint.Style.FILL);
        paint_cd.setTextSize(35f * IconHelper.k);
        paint_cd.setAntiAlias(true);       //设置抗锯齿
        paint_cd.setDither(true);
        Typeface font = Typeface.create(Typeface.MONOSPACE, Typeface.BOLD);
        paint_cd.setTypeface(font);
        paint_cd.setTextAlign(Paint.Align.CENTER);

        //技能CD text画笔
        paint_cd_text.setColor(Color.RED);
        paint_cd_text.setTextSize(30f * IconHelper.k);
        paint_cd_text.setAntiAlias(true);       //设置抗锯齿
        paint_cd_text.setDither(true);

        paint_cd_big.setColor(Color.YELLOW);
        paint_cd_big.setTextSize(18f * IconHelper.k);
        paint_cd_big.setAntiAlias(true);       //设置抗锯齿
        paint_cd_big.setDither(true);
        paint_cd_big.setTextAlign(Paint.Align.CENTER);

        //死亡斜线画笔
        paint_deadline.setColor(Color.RED);
        paint_deadline.setStyle(Paint.Style.FILL);
        paint_deadline.setStrokeWidth(7f * IconHelper.k);

        //英雄name画笔
        paint_name.setColor(Color.WHITE);
        paint_name.setStyle(Paint.Style.FILL);
        paint_name.setTextSize(25f * IconHelper.k);
        paint_name.setAntiAlias(true);       //设置抗锯齿
        paint_name.setDither(true);
        paint_name.setTextAlign(Paint.Align.CENTER);


        //兵线画笔
        paint_soldier.setColor(Color.RED);
        paint_soldier.setAlpha(240);
        paint_soldier.setStyle(Paint.Style.FILL);

        float x = ScreenUtils.getScreenWidth();
        float y = ScreenUtils.getScreenHeight();
        float center_x, center_y;
        if (x > y) {
            center_x = x / 2f;
            center_y = y / 2f;
        } else {
            center_x = y / 2f;
            center_y = x / 2f;
        }

        //野怪位置初始化
        for (int i = 0; i < 12; i++) {
            monster_location[i][0] = monster_location0[i][0] * 0.00332f * IconHelper.k;
            monster_location[i][1] = monster_location0[i][1] * -0.00332f * IconHelper.k;
        }

        while (threadFlag) {
            if (controlFlag) {
                o00o0ooo0();
                try {
//                    获取画布
                    canvas = lockCanvas();
                    canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
                    if (DataParser.status == 0) {
                        canvas.drawText("Waiting for game begin...",
                                220 * IconHelper.k,
                                30 * IconHelper.k,
                                paint_start
                        );
                        //Log.d("mylog", "Status = "+DataParser.status);
                    } else {
                        //绘制野怪
                        int cd;
                        if (DrawHelper.draw_buff) {
                            try {
                                for (int j = 0; j < 12; j++) {
                                    cd = buffArray[j].cd;
                                    //buff
                                    if (j % 3 == 0) {
                                        if (cd != 95 && cd != 0) {
                                            canvas.drawText("" + cd,
                                                    DrawHelper.icon_offset_x + monster_location[j][0],
                                                    DrawHelper.icon_offset_y + monster_location[j][1],
                                                    paint_buffText
                                            );
                                        } else if (cd == 95) {
                                            if (j == 0 || j == 6) {
                                                DrawHelper.drawConCircle(canvas, DrawHelper.icon_offset_x + monster_location[j][0],
                                                        (float) DrawHelper.icon_offset_y + monster_location[j][1], 9, paint_buffBlue, paint_edge);

                                            } else {
                                                DrawHelper.drawConCircle(canvas, DrawHelper.icon_offset_x + monster_location[j][0],
                                                        (float) DrawHelper.icon_offset_y + monster_location[j][1], 9, paint_buffRed, paint_edge);
                                            }
                                        }

                                    }

                                    //小野怪
                                    else {
                                        if (DrawHelper.draw_animal) {
                                            if (cd != 75 && cd != 0) {
                                                canvas.drawText("" + cd,
                                                        DrawHelper.icon_offset_x + monster_location[j][0],
                                                        DrawHelper.icon_offset_y + monster_location[j][1],
                                                        paint_buffYellow
                                                );
                                            } else if (cd == 75) {
                                                canvas.drawCircle(DrawHelper.icon_offset_x + monster_location[j][0],
                                                        (float) DrawHelper.icon_offset_y + monster_location[j][1], 6, paint_buffYellow);
                                            }
                                        }

                                    }

                                }
                            } catch (Exception e) {

                            }
                        }

                        int cnt = 0;
                        if (DrawHelper.draw_cd) {
                            //绘制大招cd text
                            canvas.drawText("大招:", 470 * IconHelper.k + DrawHelper.icon_offset_x, 25 * IconHelper.k, paint_cd_text);
                            //绘制技能cd text
                            canvas.drawText("技能:", 470 * IconHelper.k + DrawHelper.icon_offset_x, 100 * IconHelper.k, paint_cd_text);
                        }
                        for (DataParser.Hero hero : heroArray) {
                            Bitmap mBitmap = IconHelper.getGameIcon(hero.id);
                            Bitmap cdzBitmap = IconHelper.getSkillIcon(hero.cdzid);
                            if (mBitmap == null) continue;
                            //绘制CD
                            if (DrawHelper.draw_cd) {
                                float cd_head_x = 565 * k + cnt * 60 * k + DrawHelper.icon_offset_x;
                                float cd_head_y = 53 * k;
                                //绘制cd头像
                                DrawHelper.drawBitMapByCenter(canvas, mBitmap, cd_head_x, cd_head_y, paint);
                                if (hero.hp <= 0) {
                                    canvas.drawLine(cd_head_x - 14 * IconHelper.k, cd_head_y - 14 * IconHelper.k, cd_head_x + 14 * IconHelper.k, cd_head_y + 14 * IconHelper.k, paint_deadline);
                                }
                                //绘制大招冷却时间
                                if (hero.cdd != 0)
                                    canvas.drawText("" + hero.cdd, cd_head_x, 30 * IconHelper.k, paint_cd);
                                else
                                    canvas.drawCircle(cd_head_x, 15, 15, paint_ready);
                                //绘制技能冷却时间
                                if (hero.cdz != -1) {
                                    if (hero.cdz != 0)
                                        canvas.drawText("" + hero.cdz, cd_head_x, 102 * IconHelper.k, paint_cd);
                                    else
//                                        canvas.drawCircle(cd_head_x, 90 * IconHelper.k, 15, paint_ready);
                                        DrawHelper.drawBitMapByCenter(canvas, cdzBitmap, cd_head_x, 94 * k, paint);
                                }
                                cnt++;
                            }
                            if (hero.hp > 0) {
                                icon_x = hero.mapX + DrawHelper.icon_offset_x;
                                icon_y = hero.mapY + DrawHelper.icon_offset_y;
                                //绘制头像
                                DrawHelper.drawBitMapByCenter(canvas, mBitmap, icon_x, icon_y, paint);
                                if (hero.hc ==1 ) {
                                    canvas.drawCircle((float) (icon_x + 20 * IconHelper.k * Math.cos(((2 * 3.1415926) * 0.01 * DataProvider.timmer))),
                                            (float) (icon_y + 20 * IconHelper.k * Math.sin(((2 * 3.1415926) * 0.01 * DataProvider.timmer))), 7, paint_backhome);
                                    canvas.drawCircle((float) (icon_x + 20 * IconHelper.k * Math.cos(((2 * 3.1415926) * 0.01 * (DataProvider.timmer - 5)))),
                                            (float) (icon_y + 20 * IconHelper.k * Math.sin(((2 * 3.1415926) * 0.01 * (DataProvider.timmer - 8)))), 5, paint_backhome);
                                    canvas.drawCircle((float) (icon_x + 20 * IconHelper.k * Math.cos(((2 * 3.1415926) * 0.01 * (DataProvider.timmer - 9)))),
                                            (float) (icon_y + 20 * IconHelper.k * Math.sin(((2 * 3.1415926) * 0.01 * (DataProvider.timmer - 15)))), 3, paint_backhome);
                                }
                                //设置渐变血条
                                if (hero.hp < 0.6 && hero.hp > 0.3) {
                                    paint_ray.setColor(Color.rgb(255, 140, 0));
                                } else if (hero.hp <= 0.3) {
                                    paint_ray.setColor(Color.RED);
                                } else {
                                    paint_ray.setColor(Color.WHITE);
                                }
                                //绘制条形血条
                                if (!DrawHelper.draw_circle) {
                                    RectF rectF = DrawHelper.getRectByCenter(icon_x, icon_y + 28 * IconHelper.k, 58 * IconHelper.k, 8);
                                    canvas.drawRoundRect(rectF, 5, 5, paint_bedge);
                                    canvas.drawLine(icon_x - 25 * IconHelper.k, icon_y + 28 * IconHelper.k, icon_x - 25 * IconHelper.k + 50 * hero.hp * IconHelper.k, icon_y + 28 * IconHelper.k, paint_red);
                                    //绘制圈圈血条
                                } else {
                                    canvas.drawCircle(icon_x, icon_y, 20 * IconHelper.k, paint_gray);
                                    canvas.drawArc(icon_x - 20 * IconHelper.k, icon_y - 20 * IconHelper.k, icon_x + 20 * IconHelper.k, icon_y + 20 * IconHelper.k, -90, 360 * hero.hp, false, paint_blood);
                                }

                                ray_x = hero.rayX + DrawHelper.ray_offset_x;
                                ray_y = hero.rayY + DrawHelper.ray_offset_y;

                                //绘制射线
                                if (DrawHelper.draw_ray) {
//                                    if(ray_x == 0){
//                                        Log.e("mylog", "**************");
//                                        Log.e("mylog", "id = "+ hero.id);
//                                        Log.e("mylog", "mapX = "+ hero.mapX);
//                                        Log.e("mylog", "mapY = "+ hero.mapY);
//                                        Log.e("mylog", "rayX = "+ hero.rayX);
//                                        Log.e("mylog", "rayY = "+ hero.rayY);
//                                        Log.e("mylog", "**************");
//                                    }
                                    canvas.drawLine(center_x, center_y, ray_x, (int) (ray_y - 0.65 * hero.h), paint_ray);
                                }


                                // 绘制方框
                                if (DrawHelper.draw_rect) {

                                    float ry = (float) (ray_y - 0.65 * hero.h);
                                    int rw = (int) (hero.h * 0.7);
                                    int rh = (int) (hero.h * 1.3);

                                    // 绘制大地图技能
                                    if (hero.cdd != 0)
                                        canvas.drawText("" + hero.cdd, (float) (ray_x + 0.5 * rw - 15 * k), (float) (ry - 0.5 * rh + 15 * k), paint_cd_big);
                                    else
                                        canvas.drawCircle((float) (ray_x + 0.5 * rw - 15 * k), (float) (ry - 0.5 * rh + 15 * k), 7 * k, paint_ready);
                                    //绘制技能冷却时间
                                    if (hero.cdz != 0)
                                        canvas.drawText("" + hero.cdz, (float) (ray_x + 0.5 * rw - 15 * k), (float) (ry + 0.5 * rh - 15 * k), paint_cd_big);
                                    else
                                        canvas.drawCircle((float) (ray_x + 0.5 * rw - 15 * k), (float) (ry + 0.5 * rh - 15 * k), 7, paint_ready);
                                    //绘制方框
//                                RectF rectF = DrawHelper.getRectByCenter(ray_x, ry, rw, rh);
                                    DrawHelper.drawUnclosedRect(canvas, ray_x, ry, rw, rh, paint_rect);
//                                canvas.drawRoundRect(rectF, 10, 10, paint_rect);
                                    //绘制英雄名称
                                    canvas.drawText(IconHelper.getHeroName(hero.id), ray_x, ry - rh / 1.7f, paint_name);
                                    canvas.drawLine((int) (ray_x - hero.h * 0.35 - 7 * k), (int) (ray_y + 2 * k), (int) (ray_x - hero.h * 0.35 - 7 * k), (int) (ray_y + 2 * k - (hero.h * 1.3 + 4 * k) * hero.hp), paint_blood);

                                }

                            }
                        }

                        if (DrawHelper.draw_soldier) {           //绘制兵线
                            for (int q = 0; q < DataParser.num; q++) {
                                canvas.drawCircle(DrawHelper.icon_offset_x + DataParser.soldierArray[q].mapX, DrawHelper.icon_offset_y + DataParser.soldierArray[q].mapY, 3f, paint_soldier);
                            }
                        }
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                } finally {
                    if (canvas != null) {
                        unlockCanvasAndPost(canvas);
                    }

                    try {
                        Thread.sleep(15);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    private native void o00o0ooo0();
}