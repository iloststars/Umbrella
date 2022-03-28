package com.agoda.umbrella.data;

public class DataParser {

    public static final Hero heroArray[] = new Hero[5];
    public static final Buff buffArray[] = new Buff[12];
    public static final Soldier soldierArray[] = new Soldier[256];
    public static int num = 0;
    public static int status = 0;

    public static class Hero{
        public int id;
        public int mapX;
        public int mapY;
        public float hp;
        public int cdd;
        public int cdz;
        public int cdzid;
        public int rayX;
        public int rayY;
        public int h;
        public int distance;
        public int hc;
    }

    public static class Buff{
        public int cd;
    }

    public static class Soldier{
        public int mapX;
        public int mapY;
    }

}

