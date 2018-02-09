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

#########
# PLOTS  #
#########
#install.packages("ggpubr")
library(ggpubr)

n_rounds = 2  #for display 1, for individual plots 2
bar_label_size = 3.5 # for display 2.8, for individual plots 3.5

# CPU time v.s. Strategy
df <- data.frame(Strategy = strategies[1:(n_strategies-1)], CPU_time = t[1:(n_strategies-1)])
df$Strategy = factor(df$Strategy, levels = strategies) 
  
p <- ggplot(df, aes(Strategy, CPU_time)) +   geom_col()
p <- p + xlab("Strategy, (w,h)") + ylab("A*  CPU time (s)")
p <- p + geom_text(aes(label=round(CPU_time,n_rounds)), vjust=1.6, color="white", size=bar_label_size)
p1 <- p + ggtitle("Total CPU time spent in A*")  + theme(plot.title = element_text(hjust = 0.5))
show(p1)

# Distance v.s. Strategy
df <- data.frame(Strategy = strategies[1:(n_strategies-1)], L = (l[1:(n_strategies-1)]-l[1])/1000 )
df$Strategy = factor(df$Strategy, levels = strategies) 

p <- ggplot(df, aes(Strategy, L)) +   geom_col()
p <- p + xlab("Strategy, (w,h)") + ylab("L(w,h) - L(1,1)  (km)")
p <- p + geom_text(aes(label=round(L,n_rounds)), vjust=1.6, color="white", size=bar_label_size)
p2 <- p + ggtitle("Relative optimal path distance with respect to (1,1)")  + theme(plot.title = element_text(hjust = 0.5))
show(p2)

# Iterations v.s. Strategy
df <- data.frame(Strategy = strategies, iters = iter/10^6 )
df$Strategy = factor(df$Strategy, levels = strategies) 

p <- ggplot(df, aes(Strategy, iters)) +   geom_col()
p <- p + xlab("Strategy, (w,h)") + ylab("Milions of iterations")
p <- p + geom_text(aes(label=round(iters,n_rounds)), vjust=1.6, color="white", size=bar_label_size)
p3 <- p + ggtitle("A*'s while loop iterations")  + theme(plot.title = element_text(hjust = 0.5))
show(p3)


# Speedup (with respect to h=0, w = 1, (8,7)) v.s. Strategy
df <- data.frame(Strategy = strategies, speedup = t[n_strategies]/t)
df$Strategy = factor(df$Strategy, levels = strategies) 

p <- ggplot(df, aes(Strategy, speedup)) +   geom_col()
p <- p + xlab("Strategy, (w,h)") + ylab("A*  Speedup")
p <- p + geom_text(aes(label=round(speedup,n_rounds)), vjust=1.6, color="white", size=bar_label_size)
p4 <- p + ggtitle("A* Speedup with respect to breadth-first search")  + theme(plot.title = element_text(hjust = 0.5))
show(p4)


## Display of the plots
display <- ggarrange(p1,p4,p3,p2, 
          labels = c("A", "B", "C", "D"),
          ncol = 2, nrow = 2)
show(display)
# barplot(t[greedy_indexs])
# barplot(t[heuristic_indexs])