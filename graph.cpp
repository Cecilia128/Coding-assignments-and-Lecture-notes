#include "graph.h"
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "testing/SimpleTest.h"

Node::Node(std::string name) : name( std::move(name) ), edges() {}


Graph::Graph( const std::vector < std::pair < std::string, std::string > > & edges )
{
    for ( auto & i : edges ) {
        add_node( std::get< 0 > ( i ) );
        add_node( std::get< 1 > ( i ) );
        link( std::get< 0 > ( i ), std::get< 1 > ( i ) );
    }
}

void Graph::add_node(const std::string &name) {
    if ( inner.count( name ) == 0 ) {
        inner.insert( { name, Node { name } } );
    }
}

void Graph::link(const std::string &from, const std::string &to ) {
    auto a = inner.find( from );
    auto b = inner.find( to );
    if ( a != inner.end() && b != inner.end() ) {
        a->second.edges.push_back( &b->second );
    }
}

std::vector<std::string> Graph::dfs_order( const std::string & root ) const
{
    std::vector< std::string > result;    
    std::stack< std::string > Stack;
    Stack.push(root);
    while (!Stack.empty()) {
        std::string str = Stack.top();
        Stack.pop();
        if (!std::count(result.begin(),result.end(),str)) result.push_back(str);
        for (int j=inner.at(str).edges.size(); j>0; j--) {
            Node *i = inner.at(str).edges[j-1];
            if (!std::count(result.begin(),result.end(),i->name)) Stack.push(i->name);
        }
    }
    return result;
}

std::vector<std::string> Graph::bfs_order( const std::string & root ) const
{
    std::vector< std::string > result;
    std::queue< std::string > Queue;
    Queue.push(root);
    while (!Queue.empty()) {
        std::string str = Queue.front();
        Queue.pop();
        if (!std::count(result.begin(),result.end(),str)) result.push_back(str);
        for (Node *i : inner.at(str).edges) {
            if (!std::count(result.begin(),result.end(),i->name)) Queue.push(i->name);
        }
    }
    return result;
}


/*
 * TREE (ACYCLIC)
 * ------------------------
 *                 A
 *                / \
 *               B   C
 *              /     \
 *             D       E
 *                      \
 *                       F
 * DFS ORDER: A B D C E F
 * BFS ORDER: A B C D E F
 *
 */
static Graph TREE = Graph ( {
  { "A", "B" },
  { "A", "C" },
  { "B", "D" },
  { "C", "E" },
  { "E", "F" }
});

/*
 * RANDOM (CYCLIC)
 * ------------------------
 *
 *
 *   --> B --> C --> F
 *  /   /      ^
 * A <--       |
 *  \          |
 *   --> D --> E --> G
 *
 *
 * DFS ORDER: A B C F D E G
 * BFS ORDER: A B D C E F G
 *
 */
static Graph RANDOM = Graph ( {
    {"A", "B"},
    {"A", "D"},
    {"B", "A"},
    {"B", "C"},
    {"C", "F"},
    {"D", "E"},
    {"E", "C"},
    {"E", "G"}
});




PROVIDED_TEST("graph dfs", Graph) {
    EXPECT( TREE.dfs_order( "A" ) ==
                  (std::vector<std::string> { "A", "B", "D", "C", "E", "F"}) );

    EXPECT( RANDOM.dfs_order( "A" ) ==
                  (std::vector<std::string> { "A", "B", "C", "F", "D", "E", "G"}) );

}

PROVIDED_TEST("graph bfs", Graph) {
    EXPECT( TREE.bfs_order( "A" ) ==
                  (std::vector<std::string> { "A", "B", "C", "D", "E", "F"}) );

    EXPECT( RANDOM.bfs_order( "A" ) ==
                  (std::vector<std::string> { "A", "B", "D", "C", "E", "F", "G"}) );

}




