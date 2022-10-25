#ifndef ECS_H
#define ECS_H

#include "../Logger/Logger.h"
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
	protected:
		static int nextId;
};

class IPool {
	public:
		virtual ~IPool() {}
};

class Entity {
	private:
		int id;
	public:
		Entity(int id) : id(id) {};
    Entity(const Entity& entity) = default;
		int GetId() const;

		bool operator == (const Entity& other) const { return id == other.id; }
		bool operator != (const Entity& other) const { return id != other.id; }
    bool operator >(const Entity& other) const { return id > other.id; }
    bool operator <(const Entity& other) const { return id < other.id; }

    template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
    template <typename TComponent> void RemoveComponent();
    template <typename TComponent> bool HasComponent() const;
    template <typename TComponent> TComponent& GetComponent() const;

    class Registry* registry;
};

template <typename TComponent>
class Component: IComponent {
	private:
	
	public:
		static int GetId() {
			static auto id = nextId++;
			return id;
		}
};

class System {
	private:
		Signature componentSignature;
		std::vector<Entity> entities;
	public:
		System() = default;
		~System() = default;

		void AddEntityToSystem(Entity entity);
		void RemoveEntityFromSystem(Entity entity);
		std::vector<Entity> GetSystemEntities() const;
		const Signature& GetComponentSignature() const;

		template <typename TComponent> void RequireComponent();
};

template <typename T>
class Pool : public IPool{
	private:
		std::vector<T> data;
	public:
		Pool(int size = 100) {
			data.resize(size);
		}

		~Pool() = default;

		bool isEmpty() const {
			return data.empty();
		}

		int GetSize() const {
			return data.size();
		}

		void Resize(int n) {
			data.resize(n);
		}

		void Clear() {
			data.clear();
		}

		void Add(T object) {
			data.push_back(object);
		}

		void Set(int index, T object) {
			data[index] = object;
		}

		T& Get(int index) {
			return static_cast<T&> (data[index]);
		}

		T& operator [](unsigned int index) {
			return data[index];
		}
};

class Registry {
	private:
		int numEntities = 0;
		std::vector <std::shared_ptr<IPool>> componentPools;
		std::vector <Signature> entityComponentSignatures;
		std::unordered_map < std::type_index, std::shared_ptr<System>> systems;
		std::set<Entity> entitiesToBeAdded;
		std::set<Entity> entitiesToBeKilled;
	public:
		Registry(){
      Logger::Log("Registry constructor called");
    }
    ~Registry(){
      Logger::Log("Registry destructor called");
    }

		Entity CreateEntity();
		
    void Update();
		
    template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
    template <typename TComponent> void RemoveComponent(Entity entity);
    template <typename TComponent> bool HasComponent(Entity entity) const;
    template <typename TComponent> TComponent& GetComponent(Entity entity) const;

    template <typename TSystem, typename ...Targs> void AddSystem(Targs&& ...args);
    template <typename TSystem> void RemoveSystem();
    template <typename TSystem> bool HasSystem() const;
    template <typename TSystem> TSystem& GetSystem() const;
    
    void AddEntityToSystems(Entity entity);
};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

template <typename TSystem, typename ...Targs> 
void Registry::AddSystem(Targs&& ...args){
  std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<Targs>(args)...);
  systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}


template <typename TSystem> 
void Registry::RemoveSystem(){
  auto system = systems.find(std::type_index(typeid(TSystem)));
  systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const{
  return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem> 
TSystem& Registry::GetSystem() const{
  auto system = systems.find(std::type_index(typeid(TSystem)));
  return *(std::static_pointer_cast<TSystem>(system->second));
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  if(componentId >= static_cast<int>(componentPools.size())) {
    componentPools.resize(componentId + 1, nullptr);
  }

  if(!componentPools[componentId]){
    std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
    componentPools[componentId] = newComponentPool;
  }

  std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

  if (entityId >= componentPool->GetSize()) {
    componentPool->Resize(numEntities);
  }

  TComponent newComponent(std::forward<TArgs>(args)...);

  componentPool->Set(entityId, newComponent);

  entityComponentSignatures[entityId].set(componentId);
  
  Logger::Log("Component id: " + std::to_string(componentId) + " was added to Entity id: " + std::to_string(entityId));
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  entityComponentSignatures[entityId].set(componentId, false);

  Logger::Log("Component id: " + std::to_string(componentId) + " was removed from entyty id: " + std::to_string(entityId));
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const{
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();
  auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

  return componentPool->Get(entityId);
}

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args){
  registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent(){
  registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const{
  return registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const{
  return registry->GetComponent<TComponent>(*this);
}

#endif
