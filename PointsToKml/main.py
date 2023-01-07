# This is a sample Python script.
import simplekml
import math
import xlrd

x_pi = 3.14159265358979324 * 3000.0 / 180.0
# 圆周率π
pi = 3.1415926535897932384626
# 长半轴长度
a = 6378245.0
# 地球的角离心率
ee = 0.00669342162296594323
# 矫正参数
interval = 0.000001


def transformlat(lng, lat):
    ret = -100.0 + 2.0 * lng + 3.0 * lat + 0.2 * lat * lat + \
          0.1 * lng * lat + 0.2 * math.sqrt(math.fabs(lng))
    ret += (20.0 * math.sin(6.0 * lng * pi) + 20.0 *
            math.sin(2.0 * lng * pi)) * 2.0 / 3.0
    ret += (20.0 * math.sin(lat * pi) + 40.0 *
            math.sin(lat / 3.0 * pi)) * 2.0 / 3.0
    ret += (160.0 * math.sin(lat / 12.0 * pi) + 320 *
            math.sin(lat * pi / 30.0)) * 2.0 / 3.0
    return ret


def transformlng(lng, lat):
    ret = 300.0 + lng + 2.0 * lat + 0.1 * lng * lng + \
          0.1 * lng * lat + 0.1 * math.sqrt(math.fabs(lng))
    ret += (20.0 * math.sin(6.0 * lng * pi) + 20.0 *
            math.sin(2.0 * lng * pi)) * 2.0 / 3.0
    ret += (20.0 * math.sin(lng * pi) + 40.0 *
            math.sin(lng / 3.0 * pi)) * 2.0 / 3.0
    ret += (150.0 * math.sin(lng / 12.0 * pi) + 300.0 *
            math.sin(lng / 30.0 * pi)) * 2.0 / 3.0
    return ret


def wgs84togcj02(lng, lat):
    """
    WGS84转GCJ02(火星坐标系)
    :param lng:WGS84坐标系的经度
    :param lat:WGS84坐标系的纬度
    :return:列表
    """
    dlat = transformlat(lng - 105.0, lat - 35.0)
    dlng = transformlng(lng - 105.0, lat - 35.0)
    radlat = lat / 180.0 * pi
    magic = math.sin(radlat)
    magic = 1 - ee * magic * magic
    sqrtmagic = math.sqrt(magic)
    dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * sqrtmagic) * pi)
    dlng = (dlng * 180.0) / (a / sqrtmagic * math.cos(radlat) * pi)
    mglat = lat + dlat
    mglng = lng + dlng
    return [mglng, mglat]


def wgs84_to_gcj02(lng, lat):
    """
    WGS84转GCJ02(火星坐标系)
    :param lng:WGS84坐标系的经度
    :param lat:WGS84坐标系的纬度
    :return:列表返回
    """
    # # 判断是否在国内
    # if out_of_china(lng, lat):
    #     return lng, lat
    dlng = transformlng(lng - 105.0, lat - 35.0)
    dlat = transformlat(lng - 105.0, lat - 35.0)
    radlat = lat / 180.0 * pi
    magic = math.sin(radlat)
    magic = 1 - ee * magic * magic
    sqrtmagic = math.sqrt(magic)
    dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * sqrtmagic) * pi)
    dlng = (dlng * 180.0) / (a / sqrtmagic * math.cos(radlat) * pi)
    gclng = lng + dlng
    gclat = lat + dlat
    return [gclng, gclat]


def gcj02_to_wgs84(lng, lat):
    """
    GCJ02(火星坐标系)转GPS84
    :param lng:火星坐标系的经度
    :param lat:火星坐标系纬度
    :return:列表返回
    """
    # # 判断是否在国内
    # if out_of_china(lng, lat):
    #     return lng, lat
    dlng = transformlng(lng - 105.0, lat - 35.0)
    dlat = transformlat(lng - 105.0, lat - 35.0)
    radlat = lat / 180.0 * pi
    magic = math.sin(radlat)
    magic = 1 - ee * magic * magic
    sqrtmagic = math.sqrt(magic)
    dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * sqrtmagic) * pi)
    dlng = (dlng * 180.0) / (a / sqrtmagic * math.cos(radlat) * pi)
    wgslng = lng + dlng
    wgslat = lat + dlat

    #新加误差矫正部分
    corrent_list = wgs84_to_gcj02(wgslng, wgslat)
    clng = corrent_list[0]-lng
    clat = corrent_list[1]-lat
    dis = math.sqrt(clng*clng + clat*clat)

    while dis > interval:
        clng = clng/2
        clat = clat/2
        wgslng = wgslng - clng
        wgslat = wgslat - clat
        corrent_list = wgs84_to_gcj02(wgslng, wgslat)
        cclng = corrent_list[0] - lng
        cclat = corrent_list[1] - lat
        dis = math.sqrt(cclng*cclng + cclat*cclat)
        clng = clng if math.fabs(clng) > math.fabs(cclng) else cclng
        clat = clat if math.fabs(clat) > math.fabs(cclat) else cclat

    return [wgslng, wgslat]


def dfm_to_deg(x):
    d = int(x)  # 度 x = 3.2118
    a = x - d  # a = 0.21
    f = int(a * 100) / 60  # 分 f = 0.35
    b = x - d - a  # b = 0.0018
    m = int(b * 10000) / 3600  # 秒 m = 0.005
    return d + f + m


def points_to_kml(gps_points):
    kml = simplekml.Kml()
    line = kml.newlinestring()
    for gps in gps_points:
        lon, lat =wgs84_to_gcj02(gps[0], gps[1]) # 如果想在国产的奥维地图里显示，就转GCJ02
        # lon, lat = gps[0], gps[1]  # 如果想在谷歌的谷歌地球里显示，不用转，用WGS84
        if lat == 0 or lon == 0:
            continue

        print(lat, lon)
        line.coords.addcoordinates([(lon, lat)])
    return kml


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    input_name = "points.xls"
    output_name = "points.kml"
    print('Reading data...')
    excel = xlrd.open_workbook(input_name)  # 打开excel文件
    sheet = excel.sheet_by_index(0)
    print(sheet.name)
    num_rows = sheet.nrows  # 获取该表总行数
    print(num_rows)
    num_cols = sheet.ncols  # 获取该表总列数
    print(num_cols)

    # sheet.row_values(12)  # 获取某一行
    lon_list = sheet.col_values(12)[1:]
    lat_list = sheet.col_values(13)[1:]
    # print(lon_list)
    # print(lat_list)
    gps_point_list = []
    for i in range(len(lon_list)):
        lon_dm = math.modf(float(lon_list[i][:-2]) / 100.0)
        lat_dm = math.modf(float(lat_list[i][:-2]) / 100.0)
        lon = lon_dm[1] + (lon_dm[0] * 100) / 60
        lat = lat_dm[1] + (lat_dm[0] * 100) / 60
        # print(lon, lat)
        gps_point_list.append([lon, lat, 0])

    kml_file = points_to_kml(gps_point_list)
    kml_file.save(output_name)
