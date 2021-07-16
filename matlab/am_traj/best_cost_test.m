clear;clc;close all;
data=[
2.819942, 8031.282177, 4170.611754;
2.869942, 6593.233172, 4124.249753;
2.919942, 5630.560684, 4087.994398;
2.969942, 4990.379200, 4060.484939;
3.019942, 4568.969365, 4040.576368;
3.069942, 4295.849536, 4027.301163;
3.119942, 4123.082659, 4019.838409;
3.169942, 4018.064877, 4017.488773;
3.219942, 3958.642243, 4019.654131;
3.269942, 3929.792007, 4025.820939;
3.319942, 3921.360313, 4035.546597;
3.369942, 3926.516213, 4048.448231;
3.419942, 3940.693270, 4064.193432;
3.469942, 3960.864162, 4082.492579;
3.519942, 3985.043362, 4103.092466;
3.569942, 4011.946392, 4125.770971;
3.619942, 4040.756744, 4150.332607;
3.669942, 4070.966905, 4176.604782;
3.719942, 4102.270408, 4204.434644;
3.769942, 4134.488988, 4233.686419;
3.819942, 4167.523851, 4264.239157;
3.869942, 4201.323463, 4295.984806;
3.919942, 4235.862596, 4328.826580;
3.969942, 4271.129019, 4362.677555;
4.019942, 4307.115307, 4397.459467;
4.069942, 4343.814064, 4433.101682;
4.119942, 4381.215368, 4469.540297;
4.169942, 4419.305638, 4506.717372;
4.219942, 4458.067382, 4544.580259;
4.269942, 4497.479449, 4583.081019;
4.319942, 4537.517567, 4622.175915;
4.369942, 4578.154995, 4661.824964;
4.419942, 4619.363200, 4701.991548;
4.469942, 4661.112501, 4742.642074;
4.519942, 4703.372648, 4783.745671;
4.569942, 4746.113324, 4825.273919;
4.619942, 4789.304558, 4867.200619;
4.669942, 4832.917065, 4909.501580;
4.719942, 4876.922508, 4952.154433;
4.769942, 4921.293690, 4995.138468;
4.819942, 4966.004700, 5038.434481;
4.869942, 5011.030995, 5082.024645;
4.919942, 5056.349461, 5125.892389;
4.969942, 5101.938421, 5170.022293;
5.019942, 5147.777636, 5214.399991;
5.069942, 5193.848269, 5259.012085;
5.119942, 5240.132849, 5303.846065;
5.169942, 5286.615209, 5348.890240;

];

plot(data(:,1),data(:,2),'.-')
% legend ("acc")
hold on 
plot(data(:,1),data(:,3),'*-')
legend("acc","eval")
