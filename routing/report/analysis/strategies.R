path = "/home/dsalgador/Desktop/GitHub-Projects/AStar-Project/routing/report/analysis"
setwd(path)

file = "strategies.csv"
data = read.csv(file, header = T)

data = data.frame(w = data$Distance, h = data$Heuristic,
                  t = data$t_avg, L = data$path_distance,
                  iterations = data$iterations)

w = data$w
h = data$h
t = data$t
l = data$L
l_rel = (l-l[1])/l[1]
iter = data$iterations

n_strategies = length(w)

heuristic_indexs = 1:9
greedy_indexs = 10:length(w)

##Plots
#install.packages("ggplot2") 
library("ggplot2")

strategies = list()
for(i in 1:n_strategies){
  strategies[[i]] = paste("(", w[i], ",", h[i], ")", sep = "")
}
strategies = as.character(strategies)

# CPU time v.s. Strategy
df <- data.frame(Strategy = strategies, CPU_time = t)
df$Strategy = factor(df$Strategy, levels = strategies) 
  
p <- ggplot(df, aes(Strategy, CPU_time)) +   geom_col()
p <- p + xlab("Strategy, (w,h)") + ylab("A* CPU time (s)")
p <- p + geom_text(aes(label=round(CPU_time,2)), vjust=1.6, color="white", size=3.5)
p <- p + ggtitle("Total CPU time spent in the A* algorithm")  + theme(plot.title = element_text(hjust = 0.5))
show(p)

# Distance v.s. Strategy
df <- data.frame(Strategy = strategies, L = (l-l[1])/1000 )
df$Strategy = factor(df$Strategy, levels = strategies) 

p <- ggplot(df, aes(Strategy, L)) +   geom_col()
p <- p + xlab("Strategy, (w,h)") + ylab("L(w,h) - L(1,1) (km)")
p <- p + geom_text(aes(label=round(L,2)), vjust=1.6, color="white", size=3.5)
p <- p + ggtitle("Relative optimal path distance with respect to Haversine strategy")  + theme(plot.title = element_text(hjust = 0.5))
show(p)

# Iterations v.s. Strategy
df <- data.frame(Strategy = strategies, iters = iter/10^6 )
df$Strategy = factor(df$Strategy, levels = strategies) 

p <- ggplot(df, aes(Strategy, iters)) +   geom_col()
p <- p + xlab("Strategy, (w,h)") + ylab("Milions of iterations")
p <- p + geom_text(aes(label=round(iters,2)), vjust=1.6, color="white", size=3.5)
p <- p + ggtitle("A*'s while loop iterations")  + theme(plot.title = element_text(hjust = 0.5))
show(p)


# barplot(t[greedy_indexs])
# barplot(t[heuristic_indexs])