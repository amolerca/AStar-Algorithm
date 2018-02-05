Put node_start in the OPEN list with f (node_start) = h(node_start) (initialization)
while the OPEN list is not empty {
	Take from the open list the node node_current with the lowest f
	 			//f(node_current) = g(node_current) + h(node_current)
	if node_current is node_goal we have found the solution; break

	Generate each state node_successor that come after node_current

	for each node_successor of node_current {
		Set successor_current_cost = g(node_current) + w(node_current, node_successor)
		if node_successor is in the OPEN list {
			if g(node_successor) <= successor_current_cost continue (to line 20 )
		}//END IF 	
		else if node_successor is in the CLOSED list {
			if g(node_successor) <= successor_current_cost continue (to line 20 )
			Move node_successor from the CLOSED list to the OPEN list
		} //END ELSE IF	
		else { //it means node has not been visited: node NOT_VISITED
				Add node_successor to the OPEN list
				Set h(node_successor) to be the heuristic distance to node_goal
		} //END ELSE
			Set g(node_successor) = successor_current_cost
			Set the parent of node_successor to node_current
		} //END FOR, "line 20"
		Add node_current to the CLOSED list
	} //END WHILE
if(node_current != node_goal) exit with error (the OPEN list is empty)




//C code 
Put node_start in the OPEN list with f (node_start) = h(node_start) (initialization)
while the OPEN list is not empty {
	Take from the open list the node node_current with the lowest f
	 			//f(node_current) = g(node_current) + h(node_current)
	if node_current is node_goal we have found the solution; break
	//Until here CODE DONE

	Generate each state node_successor that come after node_current
	//we need an array of Astar nodes, successors of node_current 
	/*asnode[i].node
	id_successor_node = current_node->node->successor[i]->id
	successor_node */

	for each node_successor of node_current {
		Set successor_current_cost = g(node_current) + w(node_current, node_successor)
		if node_successor is in the OPEN list {
			if g(node_successor) <= successor_current_cost continue (to line 20 )
		} //END IF 	
		else if node_successor is in the CLOSED list {
			if g(node_successor) <= successor_current_cost continue (to line 20 )
			Move node_successor from the CLOSED list to the OPEN list
		} //END ELSE IF	
		else { //it means node has not been visited: node NOT_VISITED
				Add node_successor to the OPEN list
				Set h(node_successor) to be the heuristic distance to node_goal
		} //END ELSE
			Set g(node_successor) = successor_current_cost
			Set the parent of node_successor to node_current
		} //END FOR, "line 20"
		Add node_current to the CLOSED list
	} //END WHILE
if(node_current != node_goal) exit with error (the OPEN list is empty)	