/*
Copyright (C) 2017 IK4-IKERLAN

This file is part of OpenDiscon.
 
OpenDiscon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
OpenDiscon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with OpenDiscon. If not, see <http://www.gnu.org/licenses/>.
*/

#define NINT(a) ((a) >= 0.0 ? (int) ((a)+0.5) : ((a)-0.5))

#include "ikClwindconInputMod.h"
#include "ikClwindconWTConfig.h"
#include "OpenDiscon_EXPORT.h"
#include <stdio.h>

void OpenDiscon_EXPORT DISCON(float *DATA, int FLAG, const char *INFILE, const char *OUTNAME, char *MESSAGE) {
	int err;
	static ikClwindconWTCon con;
	double output = -12.0;
	static FILE *f = NULL;
#define Pdelta 2
	const double deratingRatio = Pdelta*1e6; /* later to be got via the supercontroller interface */
	static ikLutbl t_mpfs;
	static ikLutbl t_mpft;
	double mpfs;
	double mpft;

#if (Pdelta == 0)
/* Pdelta = 0.00 MW */
	/* Minimum pitch from speed */
	const int mpfsN = 83; /* number of points in the lookup table */
	const double w[] = {41.783182, 41.887902, 41.992622, 42.097342, 42.202061, 42.306781, 42.411501, 42.516221, 42.620940, 42.725660, 42.830380, 42.935100, 43.039819, 43.144539, 43.249259, 43.353979, 43.458698, 43.563418, 43.668138, 43.772858, 43.877577, 43.982297, 44.087017, 44.191737, 44.296456, 44.401176, 44.505896, 44.610616, 44.715335, 44.820055, 44.924775, 45.029495, 45.134214, 45.238934, 45.343654, 45.448374, 45.553093, 45.657813, 45.762533, 45.867253, 45.971972, 46.076692, 46.181412, 46.286132, 46.390852, 46.495571, 46.600291, 46.705011, 46.809731, 46.914450, 47.019170, 47.123890, 47.228610, 47.333329, 47.438049, 47.542769, 47.647489, 47.752208, 47.856928, 47.961648, 48.066368, 48.171087, 48.275807, 48.380527, 48.485247, 48.589966, 48.694686, 48.799406, 48.904126, 49.008845, 49.113565, 49.218285, 49.323005, 49.427724, 49.532444, 49.637164, 49.741884, 49.846603, 49.951323, 50.056043, 50.160763, 50.265482, 50.370202, }; /* rad/s */
	const double pitch_w[] = {-0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005712, -0.005123, -0.004535, -0.004243, -0.003951, -0.003658, -0.003366, -0.003073, -0.002781, -0.002489, -0.002197, -0.001905, -0.001614, -0.001297, -0.000908, -0.000518, -0.000129, 0.000260, 0.000649, 0.001007, 0.001359, 0.001711, 0.002064, 0.002416, 0.002742, 0.003031, 0.003320, 0.003610, 0.003899, 0.004188, 0.004508, 0.004828, 0.005148, 0.005469, 0.005789, 0.006076, 0.006335, 0.006595, 0.006854, 0.007114, 0.007361, 0.007485, 0.007609, 0.007733, 0.007857, 0.007981, 0.008226, 0.008546, 0.008866, 0.009186, 0.009506, 0.009830, 0.010178, 0.010526, 0.010874, 0.011221, 0.011569, 0.011836, 0.012057, 0.012279, 0.012500, 0.012721, 0.012950, 0.012950, }; /* rad */

	/* Minimum pitch from torque */
	const int mpftN = 200; /* number of points in the lookup table */
	const double q[] = {0.000000, 1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000, 8.000000, 9.000000, 10.000000, 11.000000, 12.000000, 13.000000, 14.000000, 15.000000, 16.000000, 17.000000, 18.000000, 19.000000, 20.000000, 21.000000, 22.000000, 23.000000, 24.000000, 25.000000, 26.000000, 27.000000, 28.000000, 29.000000, 30.000000, 31.000000, 32.000000, 33.000000, 34.000000, 35.000000, 36.000000, 37.000000, 38.000000, 39.000000, 40.000000, 41.000000, 42.000000, 43.000000, 44.000000, 45.000000, 46.000000, 47.000000, 48.000000, 49.000000, 50.000000, 51.000000, 52.000000, 53.000000, 54.000000, 55.000000, 56.000000, 57.000000, 58.000000, 59.000000, 60.000000, 61.000000, 62.000000, 63.000000, 64.000000, 65.000000, 66.000000, 67.000000, 68.000000, 69.000000, 70.000000, 71.000000, 72.000000, 73.000000, 74.000000, 75.000000, 76.000000, 77.000000, 78.000000, 79.000000, 80.000000, 81.000000, 82.000000, 83.000000, 84.000000, 85.000000, 86.000000, 87.000000, 88.000000, 89.000000, 90.000000, 91.000000, 92.000000, 93.000000, 94.000000, 95.000000, 96.000000, 97.000000, 98.000000, 99.000000, 100.000000, 101.000000, 102.000000, 103.000000, 104.000000, 105.000000, 106.000000, 107.000000, 108.000000, 109.000000, 110.000000, 111.000000, 112.000000, 113.000000, 114.000000, 115.000000, 116.000000, 117.000000, 118.000000, 119.000000, 120.000000, 121.000000, 122.000000, 123.000000, 124.000000, 125.000000, 126.000000, 127.000000, 128.000000, 129.000000, 130.000000, 131.000000, 132.000000, 133.000000, 134.000000, 135.000000, 136.000000, 137.000000, 138.000000, 139.000000, 140.000000, 141.000000, 142.000000, 143.000000, 144.000000, 145.000000, 146.000000, 147.000000, 148.000000, 149.000000, 150.000000, 151.000000, 152.000000, 153.000000, 154.000000, 155.000000, 156.000000, 157.000000, 158.000000, 159.000000, 160.000000, 161.000000, 162.000000, 163.000000, 164.000000, 165.000000, 166.000000, 167.000000, 168.000000, 169.000000, 170.000000, 171.000000, 172.000000, 173.000000, 174.000000, 175.000000, 176.000000, 177.000000, 178.000000, 179.000000, 180.000000, 181.000000, 182.000000, 183.000000, 184.000000, 185.000000, 186.000000, 187.000000, 188.000000, 189.000000, 190.000000, 191.000000, 192.000000, 193.000000, 194.000000, 195.000000, 196.000000, 197.000000, 198.000000, 199.000000, }; /* kNm */
	const double pitch_q[] = {0.058657, 0.058657, 0.059647, 0.059814, 0.061492, 0.059987, 0.060039, 0.060214, 0.060738, 0.061204, 0.060716, 0.060866, 0.060911, 0.060912, 0.060738, 0.060214, 0.059714, 0.059486, 0.059029, 0.058643, 0.057774, 0.057477, 0.056825, 0.055966, 0.054832, 0.054105, 0.053567, 0.052601, 0.051752, 0.050964, 0.050266, 0.049567, 0.048520, 0.047929, 0.046978, 0.046368, 0.045478, 0.044750, 0.043753, 0.042894, 0.042671, 0.041641, 0.041325, 0.039976, 0.039347, 0.038245, 0.037613, 0.037025, 0.036444, 0.035528, 0.034732, 0.033685, 0.032987, 0.032158, 0.031502, 0.030892, 0.029687, 0.028840, 0.028138, 0.027067, 0.026355, 0.025507, 0.025172, 0.024346, 0.023527, 0.022864, 0.021468, 0.021106, 0.020263, 0.019542, 0.018836, 0.018326, 0.016943, 0.016057, 0.016020, 0.015080, 0.014616, 0.013244, 0.012566, 0.011868, 0.011170, 0.010297, 0.009381, 0.008351, 0.007555, 0.007424, 0.006841, 0.006274, 0.005760, 0.004091, 0.003198, 0.002967, 0.002444, 0.001338, 0.000533, 0.000053, -0.001020, -0.001920, -0.002195, -0.003295, -0.004363, -0.004363, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005934, -0.005471, -0.005471, }; /* rad */

#endif
#if (Pdelta == 1)
/* Pdelta = 1.00 MW */
	/* Minimum pitch from speed */
	const int mpfsN = 43; /* number of points in the lookup table */
	const double w[] = {41.783182, 41.887902, 41.992622, 42.097342, 42.202061, 42.306781, 42.411501, 42.516221, 42.620940, 42.725660, 42.830380, 42.935100, 43.039819, 43.144539, 43.249259, 43.353979, 43.458698, 43.563418, 43.668138, 43.772858, 43.877577, 43.982297, 44.087017, 44.191737, 44.296456, 44.401176, 44.505896, 44.610616, 44.715335, 44.820055, 44.924775, 45.029495, 45.134214, 45.238934, 45.343654, 45.448374, 45.553093, 45.657813, 45.762533, 45.867253, 45.971972, 46.076692, 46.181412, }; /* rad/s */
	const double pitch_w[] = {0.076491, 0.076491, 0.076161, 0.075833, 0.075509, 0.075187, 0.074867, 0.074548, 0.074229, 0.073916, 0.073604, 0.073296, 0.072989, 0.072683, 0.072382, 0.072082, 0.071786, 0.071490, 0.071193, 0.070897, 0.070604, 0.070316, 0.070031, 0.069760, 0.069494, 0.069243, 0.068995, 0.068758, 0.068528, 0.068308, 0.068099, 0.067896, 0.067716, 0.067540, 0.067368, 0.067205, 0.067060, 0.066920, 0.066785, 0.066660, 0.066538, 0.066432, 0.066432, }; /* rad */

	/* Minimum pitch from torque */
	const int mpftN = 199; /* number of points in the lookup table */
	const double q[] = {0.000000, 1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000, 8.000000, 9.000000, 10.000000, 11.000000, 12.000000, 13.000000, 14.000000, 15.000000, 16.000000, 17.000000, 18.000000, 19.000000, 20.000000, 21.000000, 22.000000, 23.000000, 24.000000, 25.000000, 26.000000, 27.000000, 28.000000, 29.000000, 30.000000, 31.000000, 32.000000, 33.000000, 34.000000, 35.000000, 36.000000, 37.000000, 38.000000, 39.000000, 40.000000, 41.000000, 42.000000, 43.000000, 44.000000, 45.000000, 46.000000, 47.000000, 48.000000, 49.000000, 50.000000, 51.000000, 52.000000, 53.000000, 54.000000, 55.000000, 56.000000, 57.000000, 58.000000, 59.000000, 60.000000, 61.000000, 62.000000, 63.000000, 64.000000, 65.000000, 66.000000, 67.000000, 68.000000, 69.000000, 70.000000, 71.000000, 72.000000, 73.000000, 74.000000, 75.000000, 76.000000, 77.000000, 78.000000, 79.000000, 80.000000, 81.000000, 82.000000, 83.000000, 84.000000, 85.000000, 86.000000, 87.000000, 88.000000, 89.000000, 90.000000, 91.000000, 92.000000, 93.000000, 94.000000, 95.000000, 96.000000, 97.000000, 98.000000, 99.000000, 100.000000, 101.000000, 102.000000, 103.000000, 104.000000, 105.000000, 106.000000, 107.000000, 108.000000, 109.000000, 110.000000, 111.000000, 112.000000, 113.000000, 114.000000, 115.000000, 116.000000, 117.000000, 118.000000, 119.000000, 120.000000, 121.000000, 122.000000, 123.000000, 124.000000, 125.000000, 126.000000, 127.000000, 128.000000, 129.000000, 130.000000, 131.000000, 132.000000, 133.000000, 134.000000, 135.000000, 136.000000, 137.000000, 138.000000, 139.000000, 140.000000, 141.000000, 142.000000, 143.000000, 144.000000, 145.000000, 146.000000, 147.000000, 148.000000, 149.000000, 150.000000, 151.000000, 152.000000, 153.000000, 154.000000, 155.000000, 156.000000, 157.000000, 158.000000, 159.000000, 160.000000, 161.000000, 162.000000, 163.000000, 164.000000, 165.000000, 166.000000, 167.000000, 168.000000, 169.000000, 170.000000, 171.000000, 172.000000, 173.000000, 174.000000, 175.000000, 176.000000, 177.000000, 178.000000, 179.000000, 180.000000, 181.000000, 182.000000, 183.000000, 184.000000, 185.000000, 186.000000, 187.000000, 188.000000, 189.000000, 190.000000, 191.000000, 192.000000, 193.000000, 194.000000, 195.000000, 196.000000, 197.000000, 198.000000, }; /* kNm */
	const double pitch_q[] = {0.173181, 0.173181, 0.172410, 0.171656, 0.170909, 0.170173, 0.169441, 0.168716, 0.167999, 0.167317, 0.166642, 0.165969, 0.165270, 0.164568, 0.163867, 0.163170, 0.162489, 0.161815, 0.161139, 0.160470, 0.159792, 0.159121, 0.158449, 0.157789, 0.157127, 0.156423, 0.155722, 0.155022, 0.154326, 0.153629, 0.152937, 0.152247, 0.151559, 0.150881, 0.150197, 0.149505, 0.148812, 0.148110, 0.147380, 0.146664, 0.145945, 0.145226, 0.144495, 0.143773, 0.143050, 0.142336, 0.141628, 0.140936, 0.140248, 0.139562, 0.138853, 0.138141, 0.137435, 0.136725, 0.136029, 0.135331, 0.134645, 0.133953, 0.133275, 0.132597, 0.131930, 0.131274, 0.130602, 0.129911, 0.129226, 0.128542, 0.127866, 0.127189, 0.126526, 0.125864, 0.125210, 0.124341, 0.123460, 0.122590, 0.121718, 0.120849, 0.119989, 0.119145, 0.118320, 0.117508, 0.116708, 0.115928, 0.115160, 0.114403, 0.113662, 0.112910, 0.112155, 0.111416, 0.110691, 0.109975, 0.109274, 0.108586, 0.107907, 0.107240, 0.106587, 0.105943, 0.105309, 0.104672, 0.104033, 0.103390, 0.102760, 0.102139, 0.101527, 0.100927, 0.100335, 0.099753, 0.099181, 0.098617, 0.098062, 0.097517, 0.096978, 0.096450, 0.095913, 0.095370, 0.094827, 0.094291, 0.093762, 0.093244, 0.092731, 0.092226, 0.091730, 0.091239, 0.090757, 0.090282, 0.089811, 0.089351, 0.088896, 0.088447, 0.088006, 0.087567, 0.087129, 0.086691, 0.086253, 0.085825, 0.085397, 0.084972, 0.084554, 0.084138, 0.083729, 0.083320, 0.082915, 0.082516, 0.082120, 0.081728, 0.081339, 0.080954, 0.080575, 0.080198, 0.079824, 0.079453, 0.079086, 0.078724, 0.078363, 0.078006, 0.077650, 0.077300, 0.076952, 0.076607, 0.076266, 0.075925, 0.075591, 0.075259, 0.074930, 0.074603, 0.074277, 0.073957, 0.073639, 0.073326, 0.073014, 0.072705, 0.072400, 0.072098, 0.071800, 0.071504, 0.071205, 0.070909, 0.070618, 0.070330, 0.070046, 0.069775, 0.069508, 0.069255, 0.069006, 0.068767, 0.068535, 0.068311, 0.068099, 0.067893, 0.067709, 0.067529, 0.067353, 0.067188, 0.067038, 0.066895, 0.066754, 0.066627, 0.066505, 0.066394, 0.066394, }; /* rad */
#endif
#if (Pdelta == 2)
/* Pdelta = 2.00 MW */
	/* Minimum pitch from speed */
	const int mpfsN = 43; /* number of points in the lookup table */
	const double w[] = {41.783182, 41.887902, 41.992622, 42.097342, 42.202061, 42.306781, 42.411501, 42.516221, 42.620940, 42.725660, 42.830380, 42.935100, 43.039819, 43.144539, 43.249259, 43.353979, 43.458698, 43.563418, 43.668138, 43.772858, 43.877577, 43.982297, 44.087017, 44.191737, 44.296456, 44.401176, 44.505896, 44.610616, 44.715335, 44.820055, 44.924775, 45.029495, 45.134214, 45.238934, 45.343654, 45.448374, 45.553093, 45.657813, 45.762533, 45.867253, 45.971972, 46.076692, 46.181412}; /* rad/s */
	const double pitch_w[] = {0.113601, 0.113601, 0.113113, 0.112626, 0.112139, 0.111658, 0.111180, 0.110709, 0.110239, 0.109778, 0.109318, 0.108864, 0.108415, 0.107969, 0.107528, 0.107090, 0.106659, 0.106229, 0.105806, 0.105385, 0.104969, 0.104544, 0.104112, 0.103694, 0.103280, 0.102881, 0.102485, 0.102099, 0.101723, 0.101359, 0.101004, 0.100652, 0.100319, 0.099990, 0.099672, 0.099360, 0.099060, 0.098769, 0.098484, 0.098207, 0.097932, 0.097675, 0.097675}; /* rad */

	/* Minimum pitch from torque */
	const int mpftN = 177; /* number of points in the lookup table */
	const double q[] = {0.000000, 1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000, 8.000000, 9.000000, 10.000000, 11.000000, 12.000000, 13.000000, 14.000000, 15.000000, 16.000000, 17.000000, 18.000000, 19.000000, 20.000000, 21.000000, 22.000000, 23.000000, 24.000000, 25.000000, 26.000000, 27.000000, 28.000000, 29.000000, 30.000000, 31.000000, 32.000000, 33.000000, 34.000000, 35.000000, 36.000000, 37.000000, 38.000000, 39.000000, 40.000000, 41.000000, 42.000000, 43.000000, 44.000000, 45.000000, 46.000000, 47.000000, 48.000000, 49.000000, 50.000000, 51.000000, 52.000000, 53.000000, 54.000000, 55.000000, 56.000000, 57.000000, 58.000000, 59.000000, 60.000000, 61.000000, 62.000000, 63.000000, 64.000000, 65.000000, 66.000000, 67.000000, 68.000000, 69.000000, 70.000000, 71.000000, 72.000000, 73.000000, 74.000000, 75.000000, 76.000000, 77.000000, 78.000000, 79.000000, 80.000000, 81.000000, 82.000000, 83.000000, 84.000000, 85.000000, 86.000000, 87.000000, 88.000000, 89.000000, 90.000000, 91.000000, 92.000000, 93.000000, 94.000000, 95.000000, 96.000000, 97.000000, 98.000000, 99.000000, 100.000000, 101.000000, 102.000000, 103.000000, 104.000000, 105.000000, 106.000000, 107.000000, 108.000000, 109.000000, 110.000000, 111.000000, 112.000000, 113.000000, 114.000000, 115.000000, 116.000000, 117.000000, 118.000000, 119.000000, 120.000000, 121.000000, 122.000000, 123.000000, 124.000000, 125.000000, 126.000000, 127.000000, 128.000000, 129.000000, 130.000000, 131.000000, 132.000000, 133.000000, 134.000000, 135.000000, 136.000000, 137.000000, 138.000000, 139.000000, 140.000000, 141.000000, 142.000000, 143.000000, 144.000000, 145.000000, 146.000000, 147.000000, 148.000000, 149.000000, 150.000000, 151.000000, 152.000000, 153.000000, 154.000000, 155.000000, 156.000000, 157.000000, 158.000000, 159.000000, 160.000000, 161.000000, 162.000000, 163.000000, 164.000000, 165.000000, 166.000000, 167.000000, 168.000000, 169.000000, 170.000000, 171.000000, 172.000000, 173.000000, 174.000000, 175.000000, 176.000000, }; /* kNm */
	const double pitch_q[] = {0.211638, 0.211638, 0.210948, 0.210249, 0.209559, 0.208863, 0.208175, 0.207474, 0.206794, 0.206100, 0.205416, 0.204733, 0.204069, 0.203381, 0.202716, 0.202042, 0.201367, 0.200704, 0.200029, 0.199343, 0.198670, 0.197989, 0.197324, 0.196653, 0.195991, 0.195330, 0.194658, 0.193988, 0.193338, 0.192670, 0.192024, 0.191356, 0.190693, 0.190019, 0.189358, 0.188706, 0.188057, 0.187401, 0.186750, 0.185713, 0.184543, 0.183387, 0.182221, 0.181047, 0.179901, 0.178766, 0.177649, 0.176553, 0.175466, 0.174388, 0.173311, 0.172234, 0.171181, 0.170138, 0.169113, 0.168105, 0.167106, 0.166125, 0.165143, 0.164160, 0.163196, 0.162240, 0.161303, 0.160379, 0.159464, 0.158570, 0.157679, 0.156794, 0.155911, 0.155029, 0.154166, 0.153311, 0.152470, 0.151641, 0.150820, 0.150017, 0.149219, 0.148429, 0.147633, 0.146833, 0.146050, 0.145273, 0.144509, 0.143756, 0.143009, 0.142278, 0.141552, 0.140839, 0.140129, 0.139423, 0.138710, 0.137997, 0.137297, 0.136605, 0.135921, 0.135248, 0.134580, 0.133926, 0.133276, 0.132637, 0.132005, 0.131380, 0.130750, 0.130120, 0.129494, 0.128872, 0.128259, 0.127654, 0.127055, 0.126466, 0.125880, 0.125306, 0.124736, 0.124175, 0.123619, 0.123070, 0.122525, 0.121981, 0.121428, 0.120876, 0.120334, 0.119797, 0.119266, 0.118743, 0.118223, 0.117712, 0.117204, 0.116706, 0.116210, 0.115722, 0.115238, 0.114761, 0.114289, 0.113821, 0.113347, 0.112872, 0.112399, 0.111927, 0.111464, 0.111003, 0.110549, 0.110099, 0.109655, 0.109215, 0.108779, 0.108350, 0.107923, 0.107503, 0.107085, 0.106675, 0.106265, 0.105864, 0.105463, 0.105070, 0.104672, 0.104264, 0.103862, 0.103468, 0.103081, 0.102701, 0.102329, 0.101959, 0.101606, 0.101255, 0.100916, 0.100579, 0.100257, 0.099938, 0.099628, 0.099325, 0.099030, 0.098745, 0.098464, 0.098191, 0.097920, 0.097663, 0.097663, }; /* rad */
#endif

	static double t;
		
	if (NINT(DATA[0]) == 0) {
		ikClwindconWTConParams param;
		ikClwindconWTCon_initParams(&param);
		setParams(&param);
		ikClwindconWTCon_init(&con, &param);
		f = fopen("log.bin", "wb");
		/* mpfs & mpft */
		ikLutbl_init(&t_mpfs);
		ikLutbl_setPoints(&t_mpfs, mpfsN, w, pitch_w);
		ikLutbl_init(&t_mpft);
		ikLutbl_setPoints(&t_mpft, mpftN, q, pitch_q);
		t = (double) DATA[1];
	}
	
	if (((double) DATA[1]) > 0.005 + t) {
	
		t = (double) DATA[1];
		
		mpfs = ikLutbl_eval(&t_mpfs, (double) DATA[19])/3.1416*180.0; /* deg */
		mpft = ikLutbl_eval(&t_mpft, con.out.torqueDemand)/3.1416*180.0; /* deg */

		con.in.deratingRatio = deratingRatio;
		con.in.externalMaximumTorque = 198.9437; /* kNm */
		con.in.externalMinimumTorque = 0.0; /* kNm */
		con.in.externalMaximumPitch = 90.0; /* deg */
		con.in.externalMinimumPitch = mpfs > mpft ? mpfs : mpft; /* deg */
		con.in.generatorSpeed = (double) DATA[19]; /* rad/s */
		con.in.rotorSpeed = (double) DATA[20]; /* rad/s */
		con.in.maximumSpeed = 480.0/30*3.1416; /* rpm to rad/s */
		con.in.azimuth = 180.0/3.1416 * (double) DATA[59]; /* rad to deg */
		con.in.maximumIndividualPitch = 0.0; /* deg */
		con.in.yawErrorReference = 0.0; /* deg */
		con.in.yawError = 0.0; /* rad to deg */
		con.in.bladeRootMoments[0].c[0] = 1.0e-3 * (double) DATA[68]; /* Nm to kNm */
		con.in.bladeRootMoments[0].c[1] = 1.0e-3 * (double) DATA[29]; /* Nm to kNm */
		con.in.bladeRootMoments[0].c[2] = 0.0; /* kNm */
		con.in.bladeRootMoments[1].c[0] = 1.0e-3 * (double) DATA[69]; /* Nm to kNm */
		con.in.bladeRootMoments[1].c[1] = 1.0e-3 * (double) DATA[30]; /* Nm to kNm */
		con.in.bladeRootMoments[1].c[2] = 0.0; /* kNm */
		con.in.bladeRootMoments[2].c[0] = 1.0e-3 * (double) DATA[70]; /* Nm to kNm */
		con.in.bladeRootMoments[2].c[1] = 1.0e-3 * (double) DATA[31]; /* Nm to kNm */
		con.in.bladeRootMoments[2].c[2] = 0.0; /* kNm */
		
		ikClwindconInputMod(&(con.in));
		ikClwindconWTCon_step(&con);

		fwrite(&mpfs, 1, sizeof(output), f);
		fwrite(&mpft, 1, sizeof(output), f);
		fwrite(&(con.out.pitchDemandBlade1), 1, sizeof(output), f);
		err = ikClwindconWTCon_getOutput(&con, &output, "power manager>minimum pitch");
		fwrite(&output, 1, sizeof(output), f);
		
	}
	
	DATA[46] = (float) (con.out.torqueDemand*1.0e3); /* kNm to Nm */
	DATA[41] = (float) (con.out.pitchDemandBlade1/180.0*3.1416); /* deg to rad */
	DATA[42] = (float) (con.out.pitchDemandBlade2/180.0*3.1416); /* deg to rad */
	DATA[43] = (float) (con.out.pitchDemandBlade3/180.0*3.1416); /* deg to rad */
	err = ikClwindconWTCon_getOutput(&con, &output, "collective pitch demand");
	DATA[44] = (float) (output/180.0*3.1416); /* deg to rad (collective pitch angle) */
}	
