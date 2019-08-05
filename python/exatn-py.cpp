#include "exatn_service.hpp"
#include "DriverClient.hpp"
#include "numerics.hpp"

#include <pybind11/complex.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/eigen.h>
#include <pybind11/iostream.h>
#include <pybind11/operators.h>

namespace py = pybind11;
using namespace exatn;
using namespace numerics;
using namespace pybind11::literals;


/**
  Trampoline class for abstract virtual functions in TensorOperation
*/

class PyTensorOperation : public exatn::numerics::TensorOperation {
    public:
    /* Inherit the constructors */
    using TensorOperation::TensorOperation;

    void printIt() {
        PYBIND11_OVERLOAD (
            void,
            TensorOperation,
            printIt,
        );
    }

    bool isSet() {
        PYBIND11_OVERLOAD_PURE (
            bool,
            TensorOperation,
            isSet,
        );
    }


};



PYBIND11_MODULE(_pyexatn, m) {
  m.doc() =
      "Python bindings for ExaTN.";

  py::class_<BytePacket>(
      m, "BytePacket", "")
      .def_readwrite("base_addr", &BytePacket::base_addr,"")
      .def_readwrite("size_bytes", &BytePacket::size_bytes, "");

  py::class_<TensorDenseBlock>(
      m, "TensorDenseBlock", "")
      .def_readwrite("num_dims", &TensorDenseBlock::num_dims,"")
      .def_readwrite("data_kind", &TensorDenseBlock::data_kind, "")
      .def_readwrite("body_ptr", &TensorDenseBlock::body_ptr, "")
      .def_readwrite("bases", &TensorDenseBlock::bases, "")
      .def_readwrite("dims", &TensorDenseBlock::dims, "");

  py::class_<TensorMethod<Identifiable>, std::shared_ptr<TensorMethod<Identifiable>>>(
      m, "TensorMethod", "")
      .def("pack", &TensorMethod<Identifiable>::pack, "")
      .def("unpack", &TensorMethod<Identifiable>::unpack, "")
      .def("apply", &TensorMethod<Identifiable>::apply, "");

  py::class_<exatn::rpc::DriverClient, std::shared_ptr<exatn::rpc::DriverClient>>(
      m, "DriverClient","")
      .def("interpretTAProL", &exatn::rpc::DriverClient::interpretTAProL, "")
      .def("registerTensorMethod", &exatn::rpc::DriverClient::registerTensorMethod, "")
      .def("getResults", &exatn::rpc::DriverClient::getResults, "")
      .def("shutdown", &exatn::rpc::DriverClient::shutdown, "");



/**
    Numerics namespace python class bindings
 */

  py::class_<exatn::numerics::TensorOperation,
             std::shared_ptr<exatn::numerics::TensorOperation>,
             PyTensorOperation>(
      m, "TensorOperation","")
      .def("printIt", &exatn::numerics::TensorOperation::printIt, "")
      .def("isSet", &exatn::numerics::TensorOperation::isSet, "")
      .def("getNumOperands", &exatn::numerics::TensorOperation::getNumOperands, "")
      .def("getNumOperandsSet", &exatn::numerics::TensorOperation::getNumOperandsSet, "")
      .def("getTensorOperandHash", &exatn::numerics::TensorOperation::getTensorOperandHash, "")
      .def("getTensorOperand", &exatn::numerics::TensorOperation::getTensorOperand, "")
      .def("setTensorOperand", &exatn::numerics::TensorOperation::setTensorOperand, "")
      .def("getNumScalars", &exatn::numerics::TensorOperation::getNumScalars, "")
      .def("getNumScalarsSet", &exatn::numerics::TensorOperation::getNumScalarsSet, "")
      .def("getScalar", &exatn::numerics::TensorOperation::getScalar, "")
      .def("setScalar", &exatn::numerics::TensorOperation::setScalar, "")
      .def("getIndexPattern", &exatn::numerics::TensorOperation::getIndexPattern, "")
      .def("setIndexPattern", &exatn::numerics::TensorOperation::setIndexPattern, "");

  py::class_<exatn::numerics::TensorOpAdd,
             std::shared_ptr<exatn::numerics::TensorOpAdd>,
             exatn::numerics::TensorOperation>(
      m, "TensorOpAdd", "")
      .def(py::init<>())
      .def("isSet", &exatn::numerics::TensorOpAdd::isSet, "")
      .def("createNew", &exatn::numerics::TensorOpAdd::createNew, "");

  py::class_<exatn::numerics::TensorOpContract,
             std::shared_ptr<exatn::numerics::TensorOpContract>,
             exatn::numerics::TensorOperation>(
      m, "TensorOpContract", "")
      .def(py::init<>())
      .def("isSet", &exatn::numerics::TensorOpContract::isSet, "")
      .def("createNew", &exatn::numerics::TensorOpContract::createNew, "");

  py::class_<exatn::numerics::TensorOpCreate,
             std::shared_ptr<exatn::numerics::TensorOpCreate>,
             exatn::numerics::TensorOperation>(
      m, "TensorOpCreate", "")
      .def(py::init<>())
      .def("isSet", &exatn::numerics::TensorOpCreate::isSet, "")
      .def("createNew", &exatn::numerics::TensorOpCreate::createNew, "");

  py::class_<exatn::numerics::TensorOpDestroy,
             std::shared_ptr<exatn::numerics::TensorOpDestroy>,
             exatn::numerics::TensorOperation>(
      m, "TensorOpDestroy", "")
      .def(py::init<>())
      .def("isSet", &exatn::numerics::TensorOpDestroy::isSet, "")
      .def("createNew", &exatn::numerics::TensorOpDestroy::createNew, "");

  py::class_<exatn::numerics::TensorOpFactory>(
      m, "TensorOpFactory", "")
      .def("registerTensorOp", &exatn::numerics::TensorOpFactory::registerTensorOp, "")
      .def("get", &exatn::numerics::TensorOpFactory::get, "");

  py::class_<exatn::numerics::TensorOpTransform,
             std::shared_ptr<exatn::numerics::TensorOpTransform>,
             exatn::numerics::TensorOperation>(
      m, "TensorOpTransform", "")
      .def("isSet", &exatn::numerics::TensorOpTransform::isSet, "")
      .def("createNew", &exatn::numerics::TensorOpTransform::createNew, "");

  py::class_<exatn::numerics::TensorNetwork, std::shared_ptr<exatn::numerics::TensorNetwork>>(
      m, "TensorNetwork","")
      .def(py::init<>())
      .def(py::init<const std::string>())
      .def(py::init<const std::string, std::shared_ptr<exatn::numerics::Tensor>, const std::vector<exatn::numerics::TensorLeg>>())
      .def("printIt", &exatn::numerics::TensorNetwork::printIt, "")
      .def("getName", &exatn::numerics::TensorNetwork::getName, "")
      .def("isFinalized", &exatn::numerics::TensorNetwork::isFinalized, "")
      .def("getNumTensors", &exatn::numerics::TensorNetwork::getNumTensors, "")
      .def("getTensor", &exatn::numerics::TensorNetwork::getTensor, "")
      .def("appendTensor", (bool (exatn::numerics::TensorNetwork::*)(unsigned int, std::shared_ptr<exatn::numerics::Tensor>, const std::vector<exatn::numerics::TensorLeg>&))
                            &exatn::numerics::TensorNetwork::appendTensor, "")
      .def("appendTensor", (bool (exatn::numerics::TensorNetwork::*)(unsigned int, std::shared_ptr<exatn::numerics::Tensor>, const std::vector<std::pair<unsigned int, unsigned int>>&,
                            const std::vector<exatn::LegDirection>&)) &exatn::numerics::TensorNetwork::appendTensor, "")
      //need to write a lambda wrapper for double && tensornetwork argument
      //.def("appendTensorNetwork", &exatn::numerics::TensorNetwork::appendTensorNetwork, "")
      .def("reoderOutputModes", &exatn::numerics::TensorNetwork::reoderOutputModes, "")
      .def("deleteTensor", &exatn::numerics::TensorNetwork::deleteTensor, "")
      .def("mergeTensors", &exatn::numerics::TensorNetwork::mergeTensors, "")
      .def("buildFromTemplate", &exatn::numerics::TensorNetwork::buildFromTemplate, "");

py::class_<exatn::numerics::VectorSpace>(
    m, "VectorSpace","")
    .def(py::init<DimExtent>())
    .def(py::init<DimExtent, const std::string>())
    .def(py::init<DimExtent, const std::string, const std::vector<SymmetryRange>>())
    .def("getDimension", &exatn::numerics::VectorSpace::getDimension, "")
    .def("printIt", &exatn::numerics::VectorSpace::printIt,"")
    .def("getSymmetrySubranges", &exatn::numerics::VectorSpace::getSymmetrySubranges, "")
    .def("registerSymmetrySubrange", &exatn::numerics::VectorSpace::registerSymmetrySubrange, "")
    .def("getRegisteredId", &exatn::numerics::VectorSpace::getRegisteredId, "");


py::class_<exatn::numerics::Subspace>(
    m, "Subspace","")
    .def(py::init<exatn::numerics::VectorSpace*, DimOffset, DimOffset>())
    .def(py::init<exatn::numerics::VectorSpace*, std::pair<DimOffset,DimOffset>>())
    .def(py::init<exatn::numerics::VectorSpace*, DimOffset, DimOffset, const std::string &>())
    .def(py::init<exatn::numerics::VectorSpace*, std::pair<DimOffset,DimOffset>, const std::string &>())
    .def("getDimension", &exatn::numerics::Subspace::getDimension, "")
    .def("printIt", &exatn::numerics::Subspace::printIt, "")
    .def("getLowerBound", &exatn::numerics::Subspace::getLowerBound, "")
    .def("getUpperBound", &exatn::numerics::Subspace::getUpperBound, "")
    .def("getVectorSpace", &exatn::numerics::Subspace::getVectorSpace, "")
    .def("getRegisteredId", &exatn::numerics::Subspace::getRegisteredId, "");



py::class_<exatn::numerics::Tensor>(
    m, "Tensor","")
    .def(py::init<std::string>())
    .def(py::init<std::string, exatn::numerics::TensorShape, exatn::numerics::TensorSignature>())
    .def(py::init<std::string, exatn::numerics::TensorShape>())
    .def(py::init<std::string, exatn::numerics::Tensor, exatn::numerics::Tensor, std::vector<exatn::numerics::TensorLeg>>())
    //templated constructor requires integral type for TensorShape - need a definition for each templated constructor and integral type
    .def(py::init<std::string, std::initializer_list<int>, std::initializer_list<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::initializer_list<short>, std::initializer_list<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::initializer_list<long>, std::initializer_list<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::initializer_list<unsigned int>, std::initializer_list<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::initializer_list<unsigned short>, std::initializer_list<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::initializer_list<unsigned long>, std::initializer_list<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::vector<char>, std::vector<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::vector<short>, std::vector<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::vector<long>, std::vector<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::vector<unsigned int>, std::vector<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::vector<unsigned short>, std::vector<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::vector<unsigned long>, std::vector<std::pair<SpaceId,SubspaceId>>>())
    .def(py::init<std::string, std::vector<int>>())
    .def(py::init<std::string, std::vector<short>>())
    .def(py::init<std::string, std::vector<long>>())
    .def(py::init<std::string, std::vector<unsigned int>>())
    .def(py::init<std::string, std::vector<unsigned short>>())
    .def(py::init<std::string, std::vector<unsigned long>>())
    .def(py::init<std::string, std::initializer_list<int>>())
    .def(py::init<std::string, std::initializer_list<short>>())
    .def(py::init<std::string, std::initializer_list<long>>())
    .def(py::init<std::string, std::initializer_list<unsigned int>>())
    .def(py::init<std::string, std::initializer_list<unsigned short>>())
    .def(py::init<std::string, std::initializer_list<unsigned long>>())
    .def("printIt", &exatn::numerics::Tensor::printIt, "")
    .def("getRank", &exatn::numerics::Tensor::getRank, "")
    .def("getShape", &exatn::numerics::Tensor::getShape, "")
    .def("getSignature", &exatn::numerics::Tensor::getSignature, "")
    .def("getDimExtent", &exatn::numerics::Tensor::getDimExtent, "")
    .def("getDimExtents", &exatn::numerics::Tensor::getDimExtents, "")
    .def("getDimSpaceId", &exatn::numerics::Tensor::getDimSpaceId, "")
    .def("getDimSubspaceId", &exatn::numerics::Tensor::getDimSubspaceId, "")
    .def("getDimSpaceAttr", &exatn::numerics::Tensor::getDimSpaceAttr, "")
    .def("deleteDimension", &exatn::numerics::Tensor::deleteDimension, "")
    .def("appendDimension", (void (exatn::numerics::Tensor::*)(std::pair<SpaceId, SubspaceId>, DimExtent)) &exatn::numerics::Tensor::appendDimension, "")
    .def("appendDimension", (void (exatn::numerics::Tensor::*)(DimExtent)) &exatn::numerics::Tensor::appendDimension, "")
    .def("getTensorHash", &exatn::numerics::Tensor::getTensorHash, "");

py::enum_<exatn::LegDirection>(
    m, "LegDirection")
    .value("UNDIRECT", exatn::LegDirection::UNDIRECT)
    .value("INWARD", exatn::LegDirection::INWARD)
    .value("OUTWARD", exatn::LegDirection::OUTWARD)
    .export_values();

py::enum_<exatn::TensorOpCode>(
    m, "TensorOpCode")
    .value("NOOP", exatn::TensorOpCode::NOOP)
    .value("CREATE", exatn::TensorOpCode::CREATE)
    .value("DESTROY", exatn::TensorOpCode::DESTROY)
    .value("TRANSFORM", exatn::TensorOpCode::TRANSFORM)
    .value("ADD", exatn::TensorOpCode::ADD)
    .value("CONTRACT", exatn::TensorOpCode::CONTRACT)
    .export_values();

py::class_<exatn::numerics::TensorLeg>(
    m, "TensorLeg","")
    //Specify the default argument for this constructor
    .def(py::init<unsigned int, unsigned int, exatn::LegDirection>(),
         "tensor_id"_a, "dimensn_id"_a, "direction"_a=exatn::LegDirection::UNDIRECT)
    .def(py::init<const TensorLeg>())
    .def("printIt", &exatn::numerics::TensorLeg::printIt, "")
    .def("getTensorId", &exatn::numerics::TensorLeg::getTensorId,"")
    .def("getDimensionId", &exatn::numerics::TensorLeg::getDimensionId, "")
    .def("getDirection", &exatn::numerics::TensorLeg::getDirection, "")
    .def("resetConnection", &exatn::numerics::TensorLeg::resetConnection, "")
    .def("resetTensorId", &exatn::numerics::TensorLeg::resetTensorId, "")
    .def("resetDimensionId", &exatn::numerics::TensorLeg::resetDimensionId, "")
    .def("resetDirection", &exatn::numerics::TensorLeg::resetDirection, "");



py::class_<exatn::numerics::NumServer, std::shared_ptr<exatn::numerics::NumServer>>(
    m, "NumServer","")
    .def(py::init<>())
    .def("registerTensorMethod", &exatn::numerics::NumServer::registerTensorMethod, "")
    .def("getTensorMethod", &exatn::numerics::NumServer::getTensorMethod, "")
    .def("registerExternalData", &exatn::numerics::NumServer::registerExternalData, "")
    .def("getExternalData", &exatn::numerics::NumServer::getExternalData, "")
    .def("openScope", &exatn::numerics::NumServer::openScope, "")
    .def("closeScope", &exatn::numerics::NumServer::closeScope, "")
    .def("createVectorSpace",
         [](const std::string & space_name, DimExtent space_dim,
            const exatn::numerics::VectorSpace *space_ptr){
                return true;
           }
    )

    .def("getVectorSpace", &exatn::numerics::NumServer::getVectorSpace, "")
    .def("destroyVectorSpace", (void (exatn::numerics::NumServer::*)(const std::string &)) &exatn::numerics::NumServer::destroyVectorSpace, "")
    .def("destroyVectorSpace", (void (exatn::numerics::NumServer::*)(SpaceId)) &exatn::numerics::NumServer::destroyVectorSpace, "")
    //.def("createSubspace", &exatn::numerics::NumServer::createSubspace, "")
    .def("getSubspace", &exatn::numerics::NumServer::getSubspace, "")
    .def("destroySubspace", (void (exatn::numerics::NumServer::*)(const std::string &)) &exatn::numerics::NumServer::destroySubspace, "")
    .def("destroySubspace", (void (exatn::numerics::NumServer::*)(SubspaceId)) &exatn::numerics::NumServer::destroySubspace, "")
    .def("submit", (int (exatn::numerics::NumServer::*)(std::shared_ptr<TensorOperation>)) &exatn::numerics::NumServer::submit, "")
    .def("submit", (int (exatn::numerics::NumServer::*)(std::shared_ptr<TensorNetwork>)) &exatn::numerics::NumServer::submit, "")
    .def("sync", (bool (exatn::numerics::NumServer::*)(const exatn::numerics::Tensor &, bool)) &exatn::numerics::NumServer::sync, "")
    .def("sync", (bool (exatn::numerics::NumServer::*)(exatn::numerics::TensorOperation &, bool)) &exatn::numerics::NumServer::sync, "")
    .def("sync", (bool (exatn::numerics::NumServer::*)(exatn::numerics::TensorNetwork &, bool)) &exatn::numerics::NumServer::sync, "");



py::class_<exatn::numerics::TensorConn>(
    m, "TensorConn", "")
    .def(py::init<std::shared_ptr<exatn::numerics::Tensor>, unsigned int, const std::vector<exatn::numerics::TensorLeg>>())
    .def("printIt", &exatn::numerics::TensorConn::printIt, "")
    .def("getNumLegs", &exatn::numerics::TensorConn::getNumLegs, "")
    .def("getTensorId", &exatn::numerics::TensorConn::getTensorId, "")
    .def("getTensor", &exatn::numerics::TensorConn::getTensor, "")
    .def("getTensorLegs", &exatn::numerics::TensorConn::getTensorLegs, "")
    .def("getDimExtent", &exatn::numerics::TensorConn::getDimExtent, "")
    .def("getDimSpaceAttr", &exatn::numerics::TensorConn::getDimSpaceAttr, "")
    .def("resetLeg", &exatn::numerics::TensorConn::resetLeg, "")
    .def("deleteLeg", &exatn::numerics::TensorConn::deleteLeg, "")
    .def("deleteLegs", &exatn::numerics::TensorConn::deleteLegs, "")
    .def("appendLeg", (void(exatn::numerics::TensorConn::*)(std::pair<SpaceId, SubspaceId>, DimExtent, exatn::numerics::TensorLeg)) &exatn::numerics::TensorConn::appendLeg,"")
    .def("appendLeg", (void(exatn::numerics::TensorConn::*)(DimExtent, exatn::numerics::TensorLeg)) &exatn::numerics::TensorConn::appendLeg, "")
    ;

py::class_<exatn::numerics::TensorShape>(
    m, "TensorShape", "")
    .def(py::init<>())
    .def(py::init<std::initializer_list<int>>())
    .def(py::init<std::initializer_list<short>>())
    .def(py::init<std::initializer_list<long>>())
    .def(py::init<std::initializer_list<unsigned int>>())
    .def(py::init<std::initializer_list<unsigned short>>())
    .def(py::init<std::initializer_list<unsigned long>>())
    .def(py::init<std::vector<int>>())
    .def(py::init<std::vector<short>>())
    .def(py::init<std::vector<long>>())
    .def(py::init<std::vector<unsigned int>>())
    .def(py::init<std::vector<unsigned short>>())
    .def(py::init<std::vector<unsigned long>>())
    .def("printIt", &exatn::numerics::TensorShape::printIt, "")
    .def("getRank", &exatn::numerics::TensorShape::getRank, "")
    .def("getDimExtent", &exatn::numerics::TensorShape::getDimExtent, "")
    .def("getDimExtents", &exatn::numerics::TensorShape::getDimExtents, "")
    .def("resetDimension", &exatn::numerics::TensorShape::resetDimension, "")
    .def("deleteDimension", &exatn::numerics::TensorShape::deleteDimension, "")
    .def("appendDimension", &exatn::numerics::TensorShape::appendDimension, "");



py::class_<exatn::numerics::TensorSignature>(
    m, "TensorSignature", "")
    .def(py::init<>())
    .def(py::init<std::initializer_list<std::pair<SpaceId, SubspaceId>>>())
    .def(py::init<const std::vector<std::pair<SpaceId, SubspaceId>>>())
    .def(py::init<unsigned int>())
    .def("printIt", &exatn::numerics::TensorSignature::printIt, "")
    .def("getRank", &exatn::numerics::TensorSignature::getRank, "")
    .def("getDimSpaceId", &exatn::numerics::TensorSignature::getDimSpaceId, "")
    .def("getDimSubspaceId", &exatn::numerics::TensorSignature::getDimSubspaceId, "")
    .def("resetDimension", &exatn::numerics::TensorSignature::resetDimension, "")
    .def("deleteDimension", &exatn::numerics::TensorSignature::deleteDimension, "")
    .def("appendDimension", &exatn::numerics::TensorSignature::appendDimension, "")
    .def("getDimSpaceAttr", &exatn::numerics::TensorSignature::getDimSpaceAttr, "");



py::class_<exatn::numerics::SubspaceRegEntry>(
    m, "SubspaceRegEntry")
    .def(py::init<std::shared_ptr<Subspace>>());



py::class_<exatn::numerics::SubspaceRegister>(
    m, "SubspaceRegister", "")
    .def(py::init<>())
    .def("registerSubspace", &exatn::numerics::SubspaceRegister::registerSubspace, "")
    ;



py::class_<exatn::numerics::SpaceRegEntry>(
    m, "SpaceRegEntry")
    .def(py::init<std::shared_ptr<exatn::numerics::VectorSpace>>());



py::class_<exatn::numerics::SpaceRegister>(
    m, "SpaceRegister")
    .def(py::init<>())
    .def("registerSpace", &exatn::numerics::SpaceRegister::registerSpace, "")
    //.def("getSpace", (const exatn::numerics::VectorSpace (exatn::numerics::SpaceRegister::*)(SpaceId)) &exatn::numerics::SpaceRegister::getSpace, "")
    //.def("getSpace", (exatn::numerics::VectorSpace* (exatn::numerics::SpaceRegister::*)(const std::string &)) &exatn::numerics::SpaceRegister::getSpace,"")
    .def("registerSubspace", &exatn::numerics::SpaceRegister::registerSubspace, "")
    .def("getSubspace", &exatn::numerics::SpaceRegister::getSubspace, "")
   ;





py::class_<exatn::numerics::SymmetryRange>(
    m, "SymmetryRange");


py::class_<exatn::numerics::SpaceBasis>(
    m, "SpaceBasis")
    .def(py::init<DimExtent>())
    .def(py::init<DimExtent, const std::vector<exatn::numerics::SymmetryRange>>())
    .def("printIt", &exatn::numerics::SpaceBasis::printIt, "")
    .def("getDimension", &exatn::numerics::SpaceBasis::getDimension, "")
    .def("getSymmetrySubranges", &exatn::numerics::SpaceBasis::getSymmetrySubranges, "")
    .def("registerSymmetrySubrange", &exatn::numerics::SpaceBasis::registerSymmetrySubrange, "")
    ;


py::class_<exatn::numerics::BasisVector>(
    m, "BasisVector")
    .def(py::init<SubspaceId>())
    .def("printIt", &exatn::numerics::BasisVector::printIt, "");

  m.def("Initialize", (void (*)()) & exatn::initialize,
        "Initialize the exatn framework.");
  m.def("getDriverClient",[](const std::string name) -> std::shared_ptr<exatn::rpc::DriverClient> {
      return exatn::getService<exatn::rpc::DriverClient>(name);
      }, "");
  m.def("Finalize", &exatn::finalize, "Finalize the framework");

}
