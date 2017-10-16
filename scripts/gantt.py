from datetime import datetime
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from matplotlib.ticker import MaxNLocator

def formatter(y, pos):
    return 'P' + str(int(y))

df = pd.read_csv('best.txt')
# df.amin = pd.to_datetime(df.amin).astype(datetime)
# df.amax = pd.to_datetime(df.amax).astype(datetime)

ax = plt.figure().gca()
plt.gca().yaxis.set_major_formatter(ticker.FuncFormatter(formatter))
ax.xaxis.set_major_locator(MaxNLocator(integer=True))
ax.yaxis.set_major_locator(MaxNLocator(integer=True))

# ax = ax.xaxis_date()
ax.hlines(df.pid, df.start, df.start + df.duration, lw=20)
for index, row in df.iterrows():
	plt.text(row['start']+(row['duration']/2), row['pid'], 'T'+str(row['tid']), color='w', ha='center', va='center')
ax.plot(df.start, df.pid, '|', ms=25, mew=2, color='gray')
ax.plot(df.start + df.duration, df.pid, '|', ms=25, mew=2, color='gray')
# print(df)
plt.xlabel('tempo')
plt.ylabel('processador')
plt.show()